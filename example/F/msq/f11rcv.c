// f11rcv.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long type;
    char text[1];
};

int main(int argc, char *argv[], char *envp[]) {
    key_t key;
    int msgid, msgtype, k;
    char *p;
    struct message *m;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s msgid msgtype\n", argv[0]);
        exit(1);
    }
    
    msgid = strtol(argv[1], &p, 10);
    if (argv[1] == p || *p) {
        fprintf(stderr, "Value msgid %s not valid\n", argv[1]);
        exit(1);
    }
    
    msgtype = strtol(argv[2], &p, 10);
    if (argv[2] == p || *p) {
        fprintf(stderr, "Value msgtype %s not valid\n", argv[2]);
        exit(1);
    }
    
    if ((m = malloc(4096)) == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    
    if ((k = msgrcv(msgid, m, 4092, msgtype, IPC_NOWAIT)) == -1) {
        perror("msgrcv");
        free(m);
        exit(1);
    }

    printf("k=%d\n", k);
    printf("m2->type=%d, m2->text='%s'\n", m->type, m->text);
    
    free(m);
    exit(0);
}