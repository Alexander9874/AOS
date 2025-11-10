// b34.c
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int p, l;
    char buf[80];
    int fd[2];
    pipe(fd);
    if (p=fork()) {
        close(fd[1]);
        while ((l=read(fd[0], buf, 80))>0)
            write(1, buf, l);
        close(fd[0]);
        wait(NULL);
        exit(0);
    }
    else {
        close(fd[0]);
        while ((l=read(0, buf, 80))>0)
            write(fd[1], buf, l);
        close(fd[1]);
        exit(0);
    }
}
