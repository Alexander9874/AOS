#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

extern int errno;

void f(int sig) {
    static int l = 0;
    int i, j;
    l++;
    printf("Handler begin %d\n", l);
    for (j = 0; j < 0x7fff; j++)
        for (i = 0; i < 0xffff; i++);
    printf("Handler end %d\n", l);
    l--;
}

int main() {
    struct sigaction act;
    act.sa_handler = f;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER;
    sigaction(SIGQUIT, &act, NULL);
    for (;;);
}
