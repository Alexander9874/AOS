#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <bits/sigaction.h>

void disposition(int sig) {
    printf("Signal %d received\n", sig);
    return;
}

int main() {
    int ret_code;

    struct sigaction act;
    act.sa_handler = disposition;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    printf("pid %d", getpid());
    for(int i = 0; i < 32; i++)
        if((ret_code = sigaction(i, &act, NULL)) == -1)
            printf("Faild for signal %d\n", i);
     
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &act, NULL);

    while(1)
        pause();

    exit(1);
}
