// b25.c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

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
    printf("change disposition\n");
    act.sa_handler = SIG_IGN;
    sigaction(SIGHUP, &act, NULL);
    printf("delete mask\n");
    sigdelset(&mask, SIGHUP);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    sleep(1);
    exit(0);
}
