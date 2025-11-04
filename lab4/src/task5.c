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
    int fd[2];
    int cmd2_index, i;

    // if(argc < 4) {
    // }
    
    for(i = 1; i < argc; i++) {
        if(!strcmp("$", argv[i])) {
            cmd2_index = i;
            break;
        }
    }
    
    if(i == argc - 1) {
        fprintf(stderr, "Usage: %s <command> [args...] $ <command> [args...]\n", argv[0]);
        exit(1);
    }

    argv[cmd2_index++] = NULL;

    pipe(fd);

    if(fork()) {
        dup2(fd[0], 0);

        close(fd[0]);
        close(fd[1]);

        execvp(argv[cmd2_index], argv + cmd2_index);
        perror(argv[cmd2_index]);
        exit(1);  
    }
    dup2(fd[1], 1);
    dup2(fd[1], 2);

    close(fd[0]);
    close(fd[1]);
    
    execvp(argv[1], argv + 1); //execvp
    perror(argv[1]);
    exit(1);
}
