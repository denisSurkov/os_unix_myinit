#ifndef OS_UNIX_MYINIT_SUB_PROCESSES_H
#define OS_UNIX_MYINIT_SUB_PROCESSES_H

struct SubProcess {
    int pid;

    char * fullConfigPath;

    char * inPathName;
    char * outPathName;
};

#endif //OS_UNIX_MYINIT_SUB_PROCESSES_H
