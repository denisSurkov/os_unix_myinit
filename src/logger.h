#include "stdio.h"

#ifndef OS_UNIX_MYINIT_LOGGER_H
#define OS_UNIX_MYINIT_LOGGER_H

extern FILE * logfile;

void initLog();

void writeLog(const char * format, ...);

void closeLog();

#endif //OS_UNIX_MYINIT_LOGGER_H
