#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "sub_processes.h"
#include "logger.h"
#include "utils.h"

#define FORK_FAILED_PID (-1)
#define FORK_CHILD 0


void startSubProcess(struct SubProcess * subProcess) {
    int input_fd = open(subProcess->stdinFilename, O_RDONLY);
    int output_fd = open(subProcess->stdoutFilename, O_CREAT | O_WRONLY | O_TRUNC, 0666);

    dup2(input_fd, STDIN_FILENO);
    dup2(output_fd, STDOUT_FILENO);

    execvp(subProcess->program, subProcess->configLineParams);
}


void forkAndStartSubprocess(struct SubProcess * subProcess) {
    pid_t pid = fork();

    switch (pid)
    {
        case FORK_FAILED_PID:
            writeLog("fork failed; pid == %d; error %s", -1, strerror(errno));
            break;
        case FORK_CHILD:
            writeLog("starting %s", subProcess->program);

            closeThisProcessAllFiles();
            startSubProcess(subProcess);
            break;
        default:
            subProcess->pid = pid;
    }
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
