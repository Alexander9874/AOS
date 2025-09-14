#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int p_id, rc;
    printf("I am father before fork. My pid=%d ppid=%d\n",
           getpid(), getppid());
    printf("I am father. Syscall \"wait\" return %d\n",
           wait (&rc));
    if (fork()) {
        // Do not forget to recompile with -O0 or -O1
        // for (rc = 0; rc < 10000000; rc++);
        printf("I am father after fork. My pid=%d ppid=%d\n",
               getpid(), getppid());
        printf("I am father. I go wait for son\n");
        p_id = wait(&rc);
        printf("I am father. My son with pid %d exit with code Ox%x\n",
               p_id, rc);
        exit(0);
    }
    else {
        printf("I am son. My pid=%d ppid=%d\n",
               getpid(), getppid());
        printf("I am son. I exit with code 3\n");
        exit(3);
    }
}
