// b22.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int pid, rc;
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
