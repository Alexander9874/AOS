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

int main(int argc, char ** argv) {
    int fd1[2], fd2[2];
    int read_fd, write_fd;
    
    char buf[4096] = {0};
    int len;

    char * prefix;
    int prefix_len;
    
    if(argc > 1) {
        write(2, "Usage: ./t4.out\n", 16);
        exit(1);
    }

    if(!strcmp("./t4.out", argv[0])) {
        pipe(fd1);
        pipe(fd2);

        printf( "fd1[0] = %d\n"
                "fd1[1] = %d\n"
                "fd2[0] = %d\n"
                "fd2[1] = %d\n", fd1[0], fd1[1], fd2[0], fd2[1]);

        write_fd = fd1[1];
        read_fd  = fd2[0];

        prefix = "Received from SON:\n\t";
        prefix_len = 20;
    }
    else {
        read_fd  = 3; // fd1[0];
        write_fd = 6; // fd2[1];

        prefix = "Received from FATHER:\n\t";
        prefix_len = 23;

        goto son;
    }

    if (!fork()) {
        close(fd1[1]);
        close(fd2[0]);
        
        execl(argv[0], "qwerty", NULL);
        perror("execl");
        exit(1);
    }
    close(fd1[0]);
    close(fd2[1]);

    while(1) {
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
