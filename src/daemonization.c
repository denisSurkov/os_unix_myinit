#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>
#include <stdlib.h>
#include "daemonization.h"
#include "logger.h"


void closeAllFiles() {
    struct rlimit flimit;
    getrlimit(RLIMIT_NOFILE, &flimit);
    for (int fd = 0; fd <flimit.rlim_max; ++fd) {
        close(fd);
    }
}


void makeItselfDaemon() {
    if (fork() != 0) {
        exit(0);
    }

    if (getpid() != 1) {
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
    }

    setsid();

    closeAllFiles();

    chdir("/");

    initLog();
    writeLog("%s\n", "started daemon");
}
