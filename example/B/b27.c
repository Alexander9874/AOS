// b27.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

extern int errno;

void w(int sig){
    int rc;
    wait(&rc);
    printf("rc=0x%x sig=%d\n", rc, sig);
}
// for some reason it exits with 0x83
// core dump created???

void f(int sig){
    int i;
    printf("Begin Sig=%d\n", sig);
    for (i=0; i<0x7fff; i++);
    printf("End sig=%d\n", sig);
}

int main() {
    int p;
    struct sigaction act;
     
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGHUP, &act, NULL);
    
    act.sa_handler = f;
    sigaction(SIGINT, &act, NULL);
    
    act.sa_handler = SIG_DFL;
    sigaction(SIGQUIT, &act, NULL);
    
    act.sa_handler = w;
    sigaction(SIGCHLD, &act, NULL);
    
    // TYPO???
    // if (p = fork()) {
    if ((p = fork())) {
        sleep(1);
        printf("Send SIGHUP\n");
        kill(p, SIGHUP);
        sleep(1);
        printf("Send SIGINT\n");
        kill(p, SIGINT);
        sleep(1);
        printf("Send SIGQUIT\n");
        kill(p, SIGQUIT);
        sleep(1);
        exit(0);
    }
    else {
        for (;;);
    }
}