#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include "config_reading.h"
#include "string.h"
#include "sub_processes.h"

#define LOG_PATH "/tmp/myinit.log"

#define log(format, ...) \
    {                    \
        time_t __t;               \
        time(&__t);               \
        fprintf(logfp, "%s\t|\t", strtok(ctime(&__t), "\n")); \
        fprintf(logfp, format, __VA_ARGS__); \
        fflush(logfp);       \
    };


int shouldRun = 1;

FILE * logfp;

void handleSighup(int sig) {
    printf("hello from sighup");
    shouldRun = 0;
}

void initAsDaemon() {
    struct rlimit flimit;

    if (getpid() != 1) {
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
    }

    if (fork() != 0) {
        exit(0);
    }

    setsid();
    getrlimit(RLIMIT_NOFILE, &flimit);
    for (int fd = 0; fd <flimit.rlim_max; ++fd) {
        close(fd);
    }

    chdir("/");

    logfp = fopen(LOG_PATH, "a");
    if (logfp == NULL) {
        exit(-2);
    }
    log("%s\n", "started daemon");

    signal(SIGHUP, handleSighup);
}


int main(int argc, char * argv[]) {
    initAsDaemon();

    if (argc != 2) {
        printf("usage: %s pathToConfiFile", argv[0]);
        exit(-1);
    }


    size_t returnProcCount = 0;
    struct SubProcess ** processes = readConfig(argv[1], &returnProcCount);
    log("got %zu processes in %s file\n", returnProcCount, argv[1]);

    for (int i = 0; i < returnProcCount; ++i) {
        struct SubProcess * subProcess = processes[i];
        for (int j = 0; j < subProcess->paramsLength; ++j) {
            log("got %s ", subProcess->configLineParams[j]);
        }
        log("%s\n", "")
    }

    pid_t cpid;
    for (int i = 0; i < returnProcCount; ++i) {
        struct SubProcess * subProcess = processes[i];
        cpid = fork();

        switch (cpid)
        {
            case -1:
                // failed
                log("fork failed; cpid == %d", -1);
                break;
            case 0:
                // child
                log("about to start %s\n", subProcess->configLineParams[0]);

                // todo: open stdin, stdout
                int res = execlp(subProcess->configLineParams[0], subProcess->configLineParams[0], subProcess->configLineParams[1], NULL);
                exit(res);
            default:
                // parent
                subProcess->pid = cpid;
        }

    }

    while (shouldRun)
    {
        cpid=waitpid(-1, NULL, 0);   //ждем любого завершенного потомка
        for (int p=0; p<returnProcCount; p++)
        {
            if(processes[p]->pid==cpid)
            {
                //делаем что-то по завершении дочернего процесса
                log("child number %d pid %d finished\n", p, cpid)
            }
        }
    }

    fclose(logfp);
    return 0;
}
