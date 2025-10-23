#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

extern int errno;

sigset_t myset;

void showpending(int sig){
    int i;
    sigset_t mask;
    sigpending(&mask);
    printf("2345678\n");
    for(i = 2; i < 9; i++)
        printf("%d", sigismember(&mask, i));
    printf("\n");
}

void clearpending(int sig){
    int i;
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    for(i = 2; i < 9; i++)
        sigaction(i, &act, NULL);
    sigprocmask(SIG_UNBLOCK, &myset, NULL);
    sigprocmask(SIG_BLOCK, &myset, NULL);
    act.sa_handler = SIG_DFL;
    for(i = 2; i < 9; i++)
        sigaction(i, &act, NULL);
}

int main() {
    int i;
    struct sigaction act;
    sigemptyset(&myset);
    for(i = 2; i < 9; i++)
        sigaddset(&myset, i);
    act.sa_handler = showpending;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
    act.sa_handler = clearpending;
    sigaction(SIGHUP, &act, NULL);
    sigprocmask(SIG_SETMASK, &myset, NULL);
    for(;;) pause();
}