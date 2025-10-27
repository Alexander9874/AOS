#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/sigaction.h>
#include <wait.h>
#include <time.h>

void disposition(int sig) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    printf("\nsignaled %d\tsleping 3 sec\nSIGQUIT BLOCKED\n", sig);
    sleep(3);
    printf("SIGQUIT UNBLOCKED\n");

    sigemptyset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    return;
}

void done(int sig) {
    printf("\npid %d signaled %d and done\n", getpid(), sig);
    exit(0);
}

int main() {
    struct sigaction act;
    act.sa_handler = disposition;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGQUIT, &act, NULL);

    act.sa_handler = done;
    sigaction(SIGINT, &act, NULL);

    while(1) pause();

    exit(1);
}