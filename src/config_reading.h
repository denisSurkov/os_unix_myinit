#include "sub_processes.h"

#ifndef OS_UNIX_MYINIT_CONFIG_READING_H
#define OS_UNIX_MYINIT_CONFIG_READING_H

struct SubProcess ** readConfig(const char * configPathName, size_t * returnProcCount);

#endif //OS_UNIX_MYINIT_CONFIG_READING_H
