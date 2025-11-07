// task2.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>

#include "wrapper.h"

#define MSG_DATA_SIZE 24 * sizeof(char)

struct message {
    long type;
    char data[MSG_DATA_SIZE];
};

int main(int argc, char *argv[]) {
    int msgid, msgtype, msgflags, len; 
    struct message msg;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s msgid msgtype\n", argv[0]);
        exit(1);
    }
    
    msgid = _strtol_errmsg(argv[1], 10, "wrong msgid");
    msgtype = _strtol_errmsg(argv[2], 10, "wrong msgtype");

    msgflags = 0            // task 2
#ifdef NOWAIT
             | IPC_NOWAIT   // task 3
#endif /* NOWAIT */
#ifdef TRUNC
             | MSG_NOERROR  // task 4
#endif /* TRUNC */ 
             ;

#ifdef TRUNC
    srand(time(NULL));
    len = rand() % (MSG_DATA_SIZE + 1);
#else /* TRUNC */
    len = MSG_DATA_SIZE;
#endif /* TRUNC */

    len = _msgrcv(msgid, & msg, len, msgtype, msgflags);

    printf("len = %d\ntype = %ld\n->",
           len, msg.type);
    fflush(stdout);
    write(1, msg.data, len);
    printf("<-\n");
    
    exit(0);
}