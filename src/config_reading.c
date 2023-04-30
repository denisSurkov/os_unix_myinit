//
// Created by denis on 25.04.23.
//

#include "config_reading.h"
#include "sub_processes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LINE_DELIMITERS " \n"
#define MAX_ARGUMENTS_IN_LINE 4096
#define MAX_LINES_IN_FILE 1024

struct SubProcess * processOneLine(char * line, size_t len) {
    char * lineArguments[MAX_ARGUMENTS_IN_LINE];
    size_t currentIndex = 0;

    char * argument;
    argument = strtok(line, LINE_DELIMITERS);

    while (argument != NULL) {
        lineArguments[currentIndex] = argument;
        currentIndex++;
        argument = strtok(NULL, LINE_DELIMITERS);
    }

    struct SubProcess * subProcess =  malloc(sizeof(struct SubProcess));
    subProcess->pid = -1;
    subProcess->paramsLength = currentIndex;
    subProcess->configLineParams = lineArguments;

    return subProcess;
};

struct SubProcess ** readConfig(char * configPathName, size_t * returnProcCount) {
    FILE * configFp;
    char * currentLine = NULL;
    size_t len = 0;

    struct SubProcess ** processes = (struct SubProcess **) malloc(sizeof(struct SubProcess *) * MAX_LINES_IN_FILE);
    size_t processCounter = 0;

    configFp = fopen(configPathName, "r");
    if (configFp == NULL) {
        // todo: how to pass log?
        perror("error while opening config file");
        return NULL;
    }

    while (getline(&currentLine, &len, configFp) != -1) {
        struct SubProcess * subProcess = processOneLine(currentLine, len);

        processes[processCounter] = subProcess;
        processCounter++;
    }

    *returnProcCount = processCounter;
    return processes;
}

