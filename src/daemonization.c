#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "stdbool.h"
#include <string.h>
#include <sys/wait.h>
#include "daemonization.h"
#include "logger.h"
#include "sub_processes.h"
#include "config_reading.h"
#include "utils.h"


void makeThisProcessDaemon() {
    if (fork() != 0) {
        exit(0);
    }

    if (getpid() != 1) {
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
    }

    setsid();

    closeThisProcessAllFiles();

    chdir("/");

    initLog();
    writeLog("started daemon");
}


struct DaemonState * initFromConfigFile(const char * configFilename) {
    size_t returnProcCount = 0;

    struct SubProcess ** processes = readConfig(configFilename, &returnProcCount);

    if (processes == NULL) {
        return NULL;
    }

    writeLog("got %zu processes in %s file", returnProcCount, configFilename);

    struct DaemonState * state = malloc(sizeof(struct DaemonState) * 1);
    state->processes = processes;
    state->processesCount = returnProcCount;
    state->_isAlive = false;

    return state;
}


void startState(struct DaemonState * state) {
    state->_isAlive = true;

    for (int i = 0; i < state->processesCount; ++i) {
        forkAndStartSubprocess(state->processes[i]);
    }
}

void stopState(struct DaemonState * state) {
    state->_isAlive = false;
}


void runWhileStateIsAlive(struct DaemonState * state) {
    while (state->_isAlive) {
        pid_t pid = waitpid(-1, NULL, 0);

        if (!state->_isAlive) break;

        for (int i=0; i < state->processesCount && state->_isAlive; i++)
        {
            if (state->processes[i]->pid != pid) {
                continue;
            }

            writeLog("child number %d pid %d finished, starting again\n", i, pid);
            forkAndStartSubprocess(state->processes[i]);
        }
    }
}


void killItSelf(struct DaemonState * state) {
    for (int i=0; i < state->processesCount && state->_isAlive; i++) {
        freeSubProcess(state->processes[i]);
    }

    free(state->processes);
    free(state);
}


void workUntilDeath(struct DaemonState * state) {
    runWhileStateIsAlive(state);
    killItSelf(state);
}
