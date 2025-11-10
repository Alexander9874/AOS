// b22a.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

void f(int sig) {
    printf("singal received %d\n", sig);
}

int main() {
    int pid, rc;
    signal(8, f);
    signal(3, f);
    signal(9, f);
    if (pid = fork()) {
        kill(pid, 8);
        kill(pid, 3);
        kill(pid, 9);
        wait(&rc);
        printf("rc=%d (0x%x)\n", rc, rc);
    } else {
        sleep(1);
        exit(123);
    }
    exit(0);
}
