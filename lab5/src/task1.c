// task1.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#include "wrapper.h"

#define RED "\033[0;31m"
#define RESET "\033[0m"

#define MSG_DATA_SIZE 24 * sizeof(char)

struct massage {
    long type;
    char data[MSG_DATA_SIZE];
};

void PrintStat(int msgid) {
    struct msqid_ds ds;
    _msgctl(msgid, IPC_STAT, &ds);
    printf("                     Key  0x%x\n", ds.msg_perm.__key);
    printf("         Owner's user ID  %d\n", ds.msg_perm.uid);
    printf("        Owner's group ID  %d\n", ds.msg_perm.gid);
    printf("       Creator's user ID  %d\n", ds.msg_perm.cuid);
    printf("      Creator's group ID  %d\n", ds.msg_perm.cgid);
    printf("    Read/write permissio  0%o\n", ds.msg_perm.mode);
    printf("         Sequence number  %d\n\n", ds.msg_perm.__seq);
    printf("    time of last msgsnd command  %s", ctime(&ds.msg_stime));
    printf("    time of last msgsnd command  %s", ctime(&ds.msg_rtime));
    printf("            time of last change  %s", ctime(&ds.msg_ctime));
    printf("        current number of bytes on queue   %lu\n", ds.__msg_cbytes);
    printf("    number of messages currently on queue  %lu\n", ds.msg_qnum);
    printf("     max number of bytes allowed on queue  %lu\n\n", ds.msg_qbytes);
    printf("    pid of last msgsnd() %u\n", ds.msg_lspid);
    printf("    pid of last msgrcv() %u\n", ds.msg_lrpid);

    return;
}

int main(int argc, char *argv[]) {
    (void)argc;

    key_t key;
    int msgid;
    struct massage msg;

    key = _ftok(argv[0], 1);
    msgid = _msgget(key, IPC_CREAT | 0600);

    printf(RED "\tBEFORE SEND\n" RESET);
    PrintStat(msgid);

    msg.type = 2;
    memset(msg.data, 'X', MSG_DATA_SIZE);
    _msgsnd(msgid, & msg, MSG_DATA_SIZE, 0);

    msg.type = 4;
    memset(msg.data, 'Y', MSG_DATA_SIZE);
    _msgsnd(msgid, & msg, MSG_DATA_SIZE, 0);

    msg.type = 8;
    memset(msg.data, 'W', MSG_DATA_SIZE);
    _msgsnd(msgid, & msg, MSG_DATA_SIZE, 0);

    printf(RED "\t\nAFTER SEND\n" RESET);
    PrintStat(msgid);

    exit(0);
}