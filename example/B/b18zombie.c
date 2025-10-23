#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

extern int errno;

void f(int sig) {
    int pid, rc;
    pid = wait(&rc);
    printf("My son %d exited with code 0x%x\n", pid, rc);
}

int main() {
    int p, f1, n;
    char status[256], buf[4096];
    struct sigaction act;
    act.sa_handler=f;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    //sigaction(SIGCHLD, &act, NULL);
    if(!(p=fork())) exit(1);
    sleep(1);
    snprintf(status, 256, "/proc/%d/status", p);
    if((f1=open(status, O_RDONLY))!=-1)
        n=read(f1, buf, 4096), 
        write(1, buf, n),
        close(f1),
        printf("done\n");
    else
        perror(status);
    exit(0);
}
