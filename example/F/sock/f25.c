// f25.c
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

int count = 0;

void f(int sig) {
    printf("count=%d\n", count);
    exit(0);
}

int main(int argc, char * argv[], char * envp[]) {
    int sv[2], l;
    char buf[16384];
    
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        exit(1);
    }
    
    signal(SIGALRM, f);
    alarm(1);
    
    for (;;) {
        write(sv[0], buf, 65536);
        count++;
    }
    
    exit(0);
}