//#define _GNU_SOURCE
// task2.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <time.h>

int main() {
    int fd[2];
    char buf[4096] = {0};
    int len;

#ifdef NONBLOCK
    pipe2(fd, O_NONBLOCK);
#else
    pipe(fd);
#endif /* NONBLOCK */

    if (fork() == 0) {
        // son
        close(fd[0]);

        while(1) {
            len = read(STDIN_FILENO, buf, 4096);
#ifndef NONBLOCK
            if(len < 0) {
                perror("read");
                exit(1);
            }
#endif /* NONBLOCK */
            if(!len) 
                break;
            write(fd[1], buf, len);
        }
        close(fd[1]);
    }
    else {
        // father
        close(fd[1]);

        while(1) {
            len = read(fd[0], buf, 4096);
#ifndef NONBLOCK
            if(len < 0) {
                perror("read");
                exit(1);
            }
#endif /* NONBLOCK */
            if(!len) 
                break;
            write(STDOUT_FILENO, buf, len);
        }
        close(fd[0]);
    }

    exit(0);
}