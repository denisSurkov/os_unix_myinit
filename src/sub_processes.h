#ifndef OS_UNIX_MYINIT_SUB_PROCESSES_H
#define OS_UNIX_MYINIT_SUB_PROCESSES_H

#include <stdlib.h>

struct SubProcess {
    int pid;

    char * program;
    char * stdinFilename;
    char * stdoutFilename;

    size_t paramsLength;
    char ** configLineParams;
};


void startSubProcess(struct SubProcess *);

void freeSubProcess(struct SubProcess *);

#endif //OS_UNIX_MYINIT_SUB_PROCESSES_H
