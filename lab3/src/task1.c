#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

void disposition(int sig) {
    printf("Signal %d received\n", sig);
    return;
}

int main() {
    int ret_code;
    printf("pid %d", getpid());
    for(int i = 0; i < 32; i++)
        if((ret_code = signal(i, disposition)) == -1)
            printf("Faild for signal %d\n", i);
            
    signal(SIGINT, SIG_DFL); 

    while(1)
        pause();

    exit(1);
}
