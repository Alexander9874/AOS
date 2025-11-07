// task9_client.c
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

struct message {
    long type;
    long msgid;
    char data[MSG_DATA_LEN];
};

void IntDisp(int sig) {
    printf("Signal %d received\n", sig);
    exit(0);
}

int main() {
    key_t key;
    int len_request, len_answer;
    int msgid_request, msgid_answer;
    struct message request, answer;
    
    key = _ftok("/etc/passwd", 0);
    msgid_request = _msgget(key, 0600);
    key = _ftok("/etc/passwd", 9);
    msgid_answer = _msgget(key, 0600);

#ifndef NO_INT_DISP
    if ((long)signal(SIGINT, IntDisp) == -1L) {
        perror("signal");
        exit(1);
    }
#endif /* NO_INT_DISP */

    srand(time(NULL));
    request.msgid = (long)rand() << 32 | (long)rand();
    request.type = __LONG_MAX__;

    printf("msgid = %ld\nEnter you data:\n", request.msgid);
    len_request = read(0, request.data, MSG_DATA_LEN);
    if (len_request == -1) {
        perror("read");
        exit(1);
    }

    printf("\n");
    while(1) {
        _msgsnd(msgid_request, & request,
               len_request + sizeof(long), 0);
        len_answer = _msgrcv(msgid_answer, & answer,
                             MSG_DATA_LEN + sizeof(long),
                             request.msgid, MSG_NOERROR);

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