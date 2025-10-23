#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

void f(int sig) {
    printf("Segmentation Fault\n");
    exit(0);
}

int main() {
    int n = 0;
    int *p;
    struct sigaction act;
    act.sa_handler = f;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGSEGV, &act, NULL);
    p = (int *) f;
    *p = 3;
    exit(0);
}
