// b24.c
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

extern int errno;

void f(int sig) {
    printf("sig=%d\n", sig);
}

int main() {
    struct sigaction act;
    sigset_t mask;
    act.sa_handler = f;
    sigemptyset(&act.sa_mask); 
    act.sa_flags = 0;
    sigaction(SIGHUP, &act, NULL);
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    kill(getpid(), SIGHUP);
    printf("signal send\n");
    sleep(1);
    sigdelset(&mask, SIGHUP);
    printf("Delete mask\n");
    sigprocmask(SIG_SETMASK, &mask, NULL);
    sleep(1);
    exit(0);
}
