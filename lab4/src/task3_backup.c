// task3.c
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
    int fd1[2], fd2[2];
    char buf[4096] = {0};
    int len;

    int read_fd, write_fd;

    pipe(fd1);
    pipe(fd2);

    if (fork() == 0) {
        close(fd1[1]);
        close(fd2[0]);

        read_fd = fd1[0];
        write_fd = fd2[1];

        while(1) {
            len = read(read_fd, buf, 4096);
            if(len < 0) {
                perror("read");
                exit(1);
            }
            if(!len) 
                break;

            write(STDOUT_FILENO, "Received from FATHER:\n\t", 23);
            write(STDOUT_FILENO, buf, len);

            len = read(STDIN_FILENO, buf, 4096);

            if(len < 0) {
                perror("read");
                exit(1);
            }
            if(!len) 
                break;

            write(write_fd, buf, len);
        }

        close(fd1[0]);
        close(fd2[1]);    
    }
    else {
        close(fd1[0]);
        close(fd2[1]);

        write_fd = fd1[1];
        read_fd = fd2[0];

        while(1) {
            len = read(STDIN_FILENO, buf, 4096);
            if(len < 0) {
                perror("read");
                exit(1);
            }
            if(!len) 
                break;

            write(write_fd, buf, len);

            len = read(read_fd, buf, 4096);

            if(len < 0) {
                perror("read");
                exit(1);
            }
            if(!len) 
                break;

            write(STDOUT_FILENO, "Received from SON:\n\t", 20);
            write(STDOUT_FILENO, buf, len);
        }

        close(fd1[1]);
        close(fd2[0]);       
    }
    exit(0);
}
