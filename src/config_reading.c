//
// Created by denis on 25.04.23.
//

#include "config_reading.h"
#include "sub_processes.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"


struct SubProcess * processOneLine(char * line, size_t len) {
    char * inPathName = NULL;
    char * outPathName = NULL;

    size_t segmentEndIndex = len;
    size_t currentIndex = len;

    currentIndex--;
    while (line[currentIndex] != ' ') {
        currentIndex--;
    }

    outPathName = (char *) malloc(sizeof(char) * (segmentEndIndex - currentIndex));
    strncpy(outPathName, &line[currentIndex + 1], segmentEndIndex - currentIndex);

    segmentEndIndex = currentIndex;
    currentIndex--;
    while (line[currentIndex] != ' ') {
        currentIndex--;
    }

    inPathName = (char *) malloc(sizeof(char) * (segmentEndIndex - currentIndex));
    strncpy(inPathName, &line[currentIndex + 1], segmentEndIndex - currentIndex);

    fprintf(stdout, "%s %lu\n", inPathName, strlen(inPathName));
    fprintf(stdout, "%s %lu\n", outPathName, strlen(outPathName));
};

struct SubProcess * readConfig(char * configPathName) {
    FILE * configFp;
    char * currentLine = NULL;
    size_t len = 0;

    configFp = fopen(configPathName, "r");
    if (configFp == NULL) {
        perror("error while opening config file");
        return NULL;
    }

    while (getline(&currentLine, &len, configFp) != -1) {
        processOneLine(currentLine, len);
    }

    return NULL;
}

