// rask11.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include "wrapper.h"

#define MSG_DATA_LEN 1024 * sizeof(char)

struct massage {
    long type;
    long data;
};

int msgid = -1;

void IntDisp(int sig) {
    printf("Signal %d received\n", sig);
    exit(0);
}

void ExitHandle(void) {
    if(msgid != -1) {
        if (msgctl(msgid, IPC_RMID, 0)) {
            perror("msgctl");
            return;
        }
        printf("Message Queue %d removed\n", msgid);
    }

    return;
}

int main(int argc, char *argv[]) {
    key_t key;
    struct massage msg;
    
#ifndef NO_INT_DISP
        if((long)signal(SIGINT, IntDisp) == -1L) {
            perror("signal");
            exit(1);
        }
#endif /* NO_INT_DISP */

#ifndef PRIVATE_KEY
    key = _ftok(argv[0], 0);
#else
    key = IPC_PRIVATE;
#endif

    msgid = _msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    msg.data = 0;
    
    if (fork()) {
        atexit(ExitHandle);

        while (1) {
            msg.type = 1;
            printf("pid %d\t%ld\n", getpid(), msg.data);
            sleep(1);
            _msgsnd(msgid, & msg, sizeof(long), 0);
            _msgrcv(msgid, & msg, sizeof(long), 2, 0);
        }
    }

    while (1) {
        _msgrcv(msgid, &msg, sizeof(long), 1, 0);
        printf("pid %d\t%ld\n", getpid(), msg.data);
        sleep(1);
        msg.type = 2;
        msg.data++;
        msgsnd(msgid, &msg, sizeof(long), 0);
    }

    exit(1);
}