// task6_client.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "wrapper.h"

#define MSG_DATA_LEN 1024 * sizeof(char)

struct massage {
    long type;
    long msgid;
    char data[MSG_DATA_LEN];
};

int msgid = -1;

void IntDisp(int sig) {
    printf("Signal %d received\n", sig);
    exit(0);
}

void ExitHandle(void) {
    if (msgctl(msgid, IPC_RMID, 0)) {
        perror("msgctl");
        return;
    }
    printf("Message Queue %d removed\n", msgid);
    return;
}

int main(int argc, char *argv[]) {
    key_t key;
    int len, len_answer, msgid_server;
    struct massage request, answer;
    
    #ifndef NO_INT_DISP
        if ((long)signal(SIGINT, IntDisp) == -1L) {
            perror("signal");
            exit(1);
        }
    #endif /* NO_INT_DISP */

    key = _ftok("/etc/passwd", 6);
    msgid_server = _msgget(key, 0600);

    srand(time(NULL));
    key = _ftok(argv[0], rand());

    msgid = _msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    atexit(ExitHandle);

    request.msgid = msgid;
    request.type = __LONG_MAX__;

    printf("Enter you data:\n");
    len = read(0, request.data, MSG_DATA_LEN);
    if(len == -1) {
        perror("read");
        exit(1);
    }

    printf("\n");

    while(1) {
        _msgsnd(msgid_server, & request, len + sizeof(long), 0);
        len_answer = _msgrcv(msgid, & answer,
                             MSG_DATA_LEN + sizeof(long),
                             0, MSG_NOERROR);

#ifdef SINGLE_LINE_TERM
        printf("\033[1A");
        printf("\033[2K");
        fflush(stdout);
#endif /* SINGLE_LINE_TERM */

        write(1, answer.data - sizeof(long), len_answer);
        sleep(1);
    }

    exit(1);
}