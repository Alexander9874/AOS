// b20.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

extern int errno;

void f(int sig) {
    printf("Division by zero\n");
    exit(0);
}

int main() {
    int n = 0;
    struct sigaction act;
    act.sa_handler = f;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGFPE, &act, NULL);
    n = 1234 / n;
    exit(0);
}
