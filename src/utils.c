#include <sys/resource.h>
#include <unistd.h>


void closeThisProcessAllFiles() {
    struct rlimit flimit;
    getrlimit(RLIMIT_NOFILE, &flimit);

    for (int fd = 0; fd <flimit.rlim_max; ++fd) {
        close(fd);
    }
}
