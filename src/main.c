#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "config_reading.h"
#include "string.h"
#include "sub_processes.h"
#include "logger.h"
#include "daemonization.h"

int shouldRun = 1;


void handleSighup(int _) {
    printf("hello from sighup");
    shouldRun = 0;
}


int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("usage: %s pathToConfiFile", argv[0]);
        exit(-1);
    }

    makeItselfDaemon();
    signal(SIGHUP, handleSighup);

    size_t returnProcCount = 0;
    struct SubProcess ** processes = readConfig(argv[1], &returnProcCount);
    writeLog("got %zu processes in %s file\n", returnProcCount, argv[1]);

    for (int i = 0; i < returnProcCount; ++i) {
        struct SubProcess * subProcess = processes[i];
        for (int j = 0; j < subProcess->paramsLength; ++j) {
            writeLog("got %s ", subProcess->configLineParams[j]);
        }
        writeLog("%s\n", "");
    }

    pid_t cpid;
    for (int i = 0; i < returnProcCount; ++i) {
        struct SubProcess * subProcess = processes[i];
        cpid = fork();

        switch (cpid)
        {
            case -1:
                // failed
                // todo: write errno st
                writeLog("fork failed; cpid == %d", -1);
                break;
            case 0:
                writeLog("starting %s\n", subProcess->program);

                closeAllFiles();
                startSubProcess(subProcess);
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
                writeLog("child number %d pid %d finished\n", p, cpid);

                // todo: start again...
            }
        }
    }

    closeLog();
    return 0;
}
