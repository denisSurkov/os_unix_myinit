#include "config_reading.h"
#include "sub_processes.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#define LINE_DELIMITERS " \n"
#define MAX_ARGUMENTS_IN_LINE 4096
#define MAX_LINES_IN_FILE 1024

bool validateIsAbsolutePath(const char * path) {
    bool result = strlen(path) >= 1 && path[0] == '/';
    if (result == false) {
        writeLog("%s is not absolute path, cannot start subprocess", path);
        return false;
    }

    return true;
}

void freeLine(char ** lineArgs, size_t totalArgs) {
    for (int i = 0; i < totalArgs; ++i) {
        free(lineArgs[i]);
    }
}


struct SubProcess * processOneLine(char * line) {
    char ** lineArguments = malloc(sizeof(char *) * MAX_ARGUMENTS_IN_LINE);
    size_t totalArguments = 0;

    char * argument;
    argument = strtok(line, LINE_DELIMITERS);

    while (argument != NULL) {
        lineArguments[totalArguments] = malloc(sizeof(char) * (strlen(argument) + 1));
        strcpy(lineArguments[totalArguments], argument);
        totalArguments++;

        argument = strtok(NULL, LINE_DELIMITERS);
    }

    if (totalArguments < 3) {
        writeLog("failed to parse line \"%s\", found %d arguments, minimum 3", line, totalArguments);
        return NULL;
    }

    char * program = lineArguments[0];
    if (!validateIsAbsolutePath(program)) {
        freeLine(lineArguments, totalArguments);
        return NULL;
    }

    totalArguments--;
    char * stdoutFilename = lineArguments[totalArguments];
    if (!validateIsAbsolutePath(stdoutFilename)) {
        freeLine(lineArguments, totalArguments);
        return NULL;
    }
    lineArguments[totalArguments] = NULL;

    totalArguments--;
    char * stdinFilename = lineArguments[totalArguments];
    if (!validateIsAbsolutePath(stdinFilename)) {
        freeLine(lineArguments, totalArguments);
        return NULL;
    }

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

    struct SubProcess ** processes = malloc(sizeof(struct SubProcess *) * MAX_LINES_IN_FILE);
    size_t processCounter = 0;

    configFile = fopen(configPathName, "r");
    if (configFile == NULL) {
        writeLog("failed to open config file, error: %s", strerror(errno));
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
