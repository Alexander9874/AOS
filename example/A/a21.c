// a20.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int p, rc;
    printf("I am father before fork. My pid=%d ppid=%d\n",
           getpid(), getppid());
    if (fork()) {
        printf("I am father after fork. My pid=%d ppid=%d\n",
               getpid(), getppid());
        printf("I go exit\n");
        sleep(1);
        exit(0);
    }
    else {
        printf("I am son. My pid=%d ppid=%d\n",
               getpid(), getppid());
        printf("I go sleep\n");
        sleep(2);
        printf("I am son. My pid=%d ppid=%d\n",
               getpid(), getppid());
        exit(0);
    }
}
