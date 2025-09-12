#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int p, rc;
    printf("I am father before fork. My pid=%d ppid=%d\n", getpid(), getppid());
    printf("I am father. Syscall \"wait\" return %d\n", wait (&rc));
    if (fork()){
        // for (rc=0; rc<10000000; rc++);
        printf("I am father after fork. My pid=%d ppid=%d\n", getpid(), getppid());
        printf("I am father. I go wait for son\n");
        p=wait(&rc);
        printf("I am father. My son with pid %d exit with code Ox%x\n", p, rc);
        exit(0);
    }
    else{
        printf("I am son. My pid=%d ppid=%d\n", getpid(), getppid());
        printf("I am son. I exit with code 3\n");
        exit(3);
    }
}