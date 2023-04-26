#include <stdio.h>
#include "config_reading.h"
#include "sub_processes.h"

int main(int argc, char * argv[]) {
    size_t returnProcCount = 0;

    struct SubProcess ** processes = readConfig(argv[1], &returnProcCount);

    for (int i = 0; i < returnProcCount; ++i) {
        struct SubProcess * subProcess = processes[i];
        for (int j = 0; j < subProcess->paramsLength; ++j) {
            fprintf(stdout, "%s ", subProcess->configLineParams[j]);
        }
        fprintf(stdout, "\n");
    }

    return 0;
}
