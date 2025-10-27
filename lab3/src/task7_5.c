#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/sigaction.h>
#include <wait.h>


extern int errno;

void son_died(int sig) {
    int pid, ret_code;
    pid = wait(&ret_code);
    printf("Son %d died 0x%x (signal %d)\n", pid, ret_code, sig);
}

void make_son(int sig) {
    int pid = fork();
    if(!pid) {
        pause();
        exit(123);
    }
    else {
        printf("\nSon %d made (signal %d)\n", pid, sig);
        usleep(2500000);
        kill(pid, SIGUSR1);
    }
    return;
}

void done(int sig) {
    exit(0);
}

int main() {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    act.sa_handler=son_died;
    sigaction(SIGCHLD, &act, NULL);
    
    act.sa_handler=done;
    sigaction(SIGINT, &act, NULL);

    act.sa_handler=make_son;
    act.sa_flags = SA_NODEFER;
    sigaction(SIGQUIT, &act, NULL);

    while(1) pause();

    exit(0);
}