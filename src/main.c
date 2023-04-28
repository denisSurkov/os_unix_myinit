#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "config_reading.h"
#include "sub_processes.h"

int shouldRun = 1;

void handleSighup(int sig) {
    printf("hello from sighup");
    shouldRun = 0;
}


int main(int argc, char * argv[]) {
    size_t returnProcCount = 0;

    signal(SIGHUP, handleSighup);

    struct SubProcess ** processes = readConfig(argv[1], &returnProcCount);

    for (int i = 0; i < returnProcCount; ++i) {
        struct SubProcess * subProcess = processes[i];
        for (int j = 0; j < subProcess->paramsLength; ++j) {
            fprintf(stdout, "%s ", subProcess->configLineParams[j]);
        }
        fprintf(stdout, "\n");
    }

    pid_t cpid;
    for (int i = 0; i < returnProcCount; ++i) {
        struct SubProcess * subProcess = processes[i];
        cpid = fork();

        switch (cpid)
        {
            case -1:
                // failed
                printf("Fork failed; cpid == -1\n");
                break;
            case 0:
                // child
                cpid = getpid();         //global PID
                execlp(subProcess->configLineParams[0], subProcess->configLineParams[0], subProcess->configLineParams[1], NULL);
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
                printf("Child number %d pid %d finished\n",p,cpid);
            }
        }
    }

    return 0;
}
