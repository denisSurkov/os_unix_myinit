#ifndef OS_UNIX_MYINIT_SUB_PROCESSES_H
#define OS_UNIX_MYINIT_SUB_PROCESSES_H

#include <stdlib.h>

struct SubProcess {
    int pid;

    size_t paramsLength;
    char ** configLineParams;
};

#endif //OS_UNIX_MYINIT_SUB_PROCESSES_H
