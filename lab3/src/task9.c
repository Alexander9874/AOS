#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/sigaction.h>
#include <wait.h>
#include <time.h>

void alarm_disp(int sig) {
    printf("signaled %d\n", sig);
    return;
}

void alarm1() {
    struct sigaction act;
    act.sa_handler = alarm_disp;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGALRM, &act, NULL);
        
    alarm(1);
    pause();
    exit(123);
}

void alarm2() {
    alarm(1);
    pause();
    exit(123);
}

int main() {
    int pid, ret_code;
    if((pid = fork())) {
        wait(&ret_code);
        printf("Son %d died 0x%x\n", pid, ret_code);
    }
    else {
        alarm1();
    }

    if((pid = fork())) {
        wait(&ret_code);
        printf("Son %d died 0x%x\n", pid, ret_code);
    }
    else {
        alarm2();
    }
    exit(0);
}