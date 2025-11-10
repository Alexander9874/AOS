// b16.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

extern int errno;

void f(int sig) {
    int i, j;
    printf("Handler begin\n");
    for (i = 0; i < 0x7fff; i++) {
        for (j = 0; j < 0xffff; j++);
    }
    printf("Handler end\n");
}

int main() {
    struct sigaction act;
    act.sa_handler = f;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGQUIT, &act, NULL);
    for (;;);
}
