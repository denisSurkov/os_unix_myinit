#ifndef OS_UNIX_MYINIT_DAEMONIZATION_H
#define OS_UNIX_MYINIT_DAEMONIZATION_H

#include "stdbool.h"

struct DaemonState {
    struct SubProcess ** processes;
    size_t processesCount;

    bool _isAlive;
};


void makeThisProcessDaemon();

struct DaemonState * initFromConfigFile(const char * configFilename);

void startState(struct DaemonState * state);

void workUntilDeath(struct DaemonState * state);

void stopState(struct DaemonState * state);

#endif //OS_UNIX_MYINIT_DAEMONIZATION_H
