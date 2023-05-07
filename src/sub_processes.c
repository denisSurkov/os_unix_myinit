//
// Created by denis on 25.04.23.
//

#include <fcntl.h>
#include <unistd.h>
#include "sub_processes.h"


void startSubProcess(struct SubProcess * subProcess) {
    int input_fd = open("input.txt", O_RDONLY);
    int output_fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);

    dup2(input_fd, STDIN_FILENO);
    dup2(output_fd, STDOUT_FILENO);

    execvp(subProcess->program, subProcess->configLineParams);
}

void freeSubProcess(struct SubProcess * subProcess) {
    free(subProcess->stdinFilename);
    free(subProcess->stdoutFilename);

    for (int i = 0; i < subProcess->paramsLength; ++i) {
        free(subProcess->configLineParams[i]);
    }

    free(subProcess->configLineParams);
    free(subProcess);
}
