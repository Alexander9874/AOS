// b45run.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

void handler() {
    execl("b45", "b45", NULL);
    perror("exec");
    exit(1);
}

int main(int argc, char * argv[], char * envp[]){
    int l, m;
    char * f;
    if (argc!=2){
        fprintf(stderr, "Usage: %s count\n", argv[0]);
        exit(1);
    }
    l=strtol(argv[1], &f, 10);
    if (argv[1]==f || *f){
        fprintf(stderr, "Count '%s' incorrect\n", argv[1]);
        exit(1);
    }
    if (l<1 || l>1000) exit(0);
    signal(SIGUSR1, handler);
    while (l-->0){
        m=fork();
        if (m==-1){
            perror("fork");
            exit(1);
        }
        if (!m)
            pause();
    }
    sleep(1);
    signal(SIGUSR1, SIG_IGN);
    kill(0, SIGUSR1);
    exit(0);
}