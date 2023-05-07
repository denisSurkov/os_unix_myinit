#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "logger.h"

#define LOG_PATH "/tmp/myinit.log"

FILE * logfile = NULL;

void initLog() {
    logfile = fopen(LOG_PATH, "a");
    if (logfile == NULL) {
        exit(-2);
    }
}


void writeLog(const char * format, ...) {
    time_t _t;
    time(&_t);
    fprintf(logfile, "%s\t|\t", strtok(ctime(&_t), "\n"));

    va_list arg_list;
    va_start(arg_list, format);
    vfprintf(logfile, format, arg_list);
    va_end(arg_list);

    fflush(logfile);
}


void closeLog() {
    fclose(logfile);
}
