// b17a.c
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int errno;

void f(int sig) {
    sigset_t mask;
    static int l = 0;
    int i, j;
    l++;
    printf("Handler begin %d\n", l);
    for(i = 0; i < 0x7fff; i++)
    for(j = 0; j < 0xffff; j++);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    printf("Handler end %d\n", l);
    l--;
}

int main() {
    struct sigaction act;
    act.sa_handler = f;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGUSR1);
    // sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);
    act.sa_flags = SA_NODEFER;
    sigaction(SIGQUIT, &act, NULL);
    
    for(;;);
}
