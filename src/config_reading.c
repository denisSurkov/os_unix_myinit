#include "config_reading.h"
#include "sub_processes.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define LINE_DELIMITERS " \n"
#define MAX_ARGUMENTS_IN_LINE 4096
#define MAX_LINES_IN_FILE 1024


struct SubProcess * processOneLine(char * line) {
    char * lineArguments[MAX_ARGUMENTS_IN_LINE];
    size_t totalArguments = 0;

    char * argument;
    argument = strtok(line, LINE_DELIMITERS);

    while (argument != NULL) {
        lineArguments[totalArguments] = argument;
        totalArguments++;
        argument = strtok(NULL, LINE_DELIMITERS);
    }

    if (totalArguments < 3) {
        writeLog("failed to parse line \"%s\", found %d arguments, minimum 3", line, totalArguments);
        return NULL;
    }


    // todo: add checks for filenames
    char * program = lineArguments[0];

    totalArguments--;
    char * stdoutFilename = lineArguments[totalArguments];
    lineArguments[totalArguments] = NULL;

    totalArguments--;
    char * stdinFilename = lineArguments[totalArguments];
    lineArguments[totalArguments] = NULL;

    struct SubProcess * subProcess =  malloc(sizeof(struct SubProcess));
    subProcess->pid = -1;
    subProcess->program = program;
    subProcess->stdinFilename = stdinFilename;
    subProcess->stdoutFilename= stdoutFilename;

    subProcess->paramsLength = totalArguments;
    subProcess->configLineParams = lineArguments;

    return subProcess;
}


struct SubProcess ** readConfig(const char * configPathName, size_t * returnProcCount) {
    FILE * configFile;
    char * currentLine = NULL;
    size_t len = 0;

    struct SubProcess ** processes = (struct SubProcess **) malloc(sizeof(struct SubProcess *) * MAX_LINES_IN_FILE);
    size_t processCounter = 0;

    configFile = fopen(configPathName, "r");
    if (configFile == NULL) {
        writeLog("failed to open config file, error: %s\n", strerror(errno));
        return NULL;
    }

    while (getline(&currentLine, &len, configFile) != -1) {
        struct SubProcess * subProcess = processOneLine(currentLine);
        if (subProcess == NULL) {
            continue;
        }
        processes[processCounter] = subProcess;
        processCounter++;
    }

    *returnProcCount = processCounter;
    return processes;
}
