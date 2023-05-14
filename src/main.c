#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include "config_reading.h"
#include "sub_processes.h"
#include "logger.h"
#include "daemonization.h"

struct DaemonState * globalState;
bool daemonsWorking = true;

const char * configFile;


void handleConfigReload(int _) {
    writeLog("got sighup signal, reloading config\n");

    stopState(globalState);

    writeLog("stopped previous daemon state\n");

    globalState = initFromConfigFile(configFile);
    startState(globalState);

    writeLog("reloaded config and start new processes\n");
}


void checkArguments(int argc, char * argv[]) {
    if (argc != 2) {
        printf("usage: %s pathToConfiFile", argv[0]);
        exit(-1);
    }
}


void registerConfigReloadSignal() {
    signal(SIGHUP, handleConfigReload);
}


int main(int argc, char * argv[]) {
    checkArguments(argc, argv);
    configFile = argv[1];

    makeThisProcessDaemon();
    registerConfigReloadSignal();

    globalState = initFromConfigFile(configFile);
    startState(globalState);

    while (daemonsWorking) {
        workUntilDeath(globalState);
    }

    closeLog();
    return 0;
}
