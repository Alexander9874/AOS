// task10_server.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "wrapper.h"

#define MSG_DATA_LEN 1024 * sizeof(char)

int msgid = -1;

struct message {
    long type;
    long msgid;
    char data[MSG_DATA_LEN];
};

void IntDisp(int sig) {
    printf("Signal %d received\n", sig);
    exit(0);
}

void DispSigChld() {
    int ret_code;
    wait(& ret_code);
    if(ret_code) {
        fprintf(stderr, "SIGCHLD return code %d\n", ret_code);
        msgid = -1;
        exit(1);
    }
    return;
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

void Answer(struct message * msg, int len) {
    msg->data[len - sizeof(long)] = '\0';
    msg->type = msg->msgid;
    msg->msgid = 0x09;

    srand(time(NULL));
    long rand_val = (long)rand() << 32 | (long)rand();
    for (int i = 0; i < 7; i++) {
        msg->msgid <<= 8;
        msg->msgid += rand_val % 0xA + 0x30;
        rand_val >>= 8;
    }

    for(int i = 0; msg->data[i]; i++) {
        if(msg->data[i] > 0x60 && msg->data[i] < 0x7B) {
            msg->data[i] -= 0x20;
        }
        else 
            if(msg->data[i] > 0x40 && msg->data[i] < 0x5B) {
                msg->data[i] += 0x20;
            }        
    }

    _msgsnd(msgid, msg, len, 0);
    msgid = -1;

    exit(0);
}

int main() {
    key_t key;
    int len;
    struct message msg;
   
    atexit(ExitHandle);
    key = _ftok("/etc/passwd", 10);
    msgid = _msgget(key, IPC_CREAT | IPC_EXCL | 0666);

#ifndef NO_INT_DISP
    if((long)signal(SIGINT, IntDisp) == -1L) {
        perror("signal");
        exit(1);
    }
#endif /* NO_INT_DISP */

#ifndef NO_SIGCHLD_DISP
    if ((long)signal(SIGCHLD, DispSigChld) == -1L) {
        perror("signal");
        exit(1);
    }
#endif /* NO_SIGCHLD_DISP */

    while(1) {
        len = _msgrcv(msgid, & msg,
                     MSG_DATA_LEN + sizeof(long),
                     __LONG_MAX__, MSG_NOERROR);
        if (!fork()) {
            Answer(& msg, len);
        }
    }

    exit(1);
}