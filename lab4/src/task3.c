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

    char * prefix;
    int prefix_len;

    int read_fd, write_fd;

    pipe(fd1);
    pipe(fd2);

    if (fork() == 0) {
        close(fd1[1]);
        close(fd2[0]);

        read_fd = fd1[0];
        write_fd = fd2[1];

        prefix = "Received from FATHER:\n\t";
        prefix_len = 23;
        
        goto son;
    }
    else {
        close(fd1[0]);
        close(fd2[1]);

        write_fd = fd1[1];
        read_fd = fd2[0];

        prefix = "Received from SON:\n\t";
        prefix_len = 20;
        
        goto father;
    }

    while(1) {
father:
        len = read(STDIN_FILENO, buf, 4096);
        if(len < 0) {
            perror("read");
            exit(1);
        }
        if(!len) 
            break;

        write(write_fd, buf, len);
son:
        len = read(read_fd, buf, 4096);

        if(len < 0) {
            perror("read");
            exit(1);
        }
        if(!len) 
            break;

        write(STDOUT_FILENO, prefix, prefix_len);
        write(STDOUT_FILENO, buf, len);
    }

    close(read_fd);
    close(write_fd);

    exit(0);
}
