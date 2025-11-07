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
    int msgid, k, i;
    struct message *m1, *m2;
    
    if ((key = ftok(argv[0], 'A')) == -1) {
        perror(argv[0]);
        exit(1);
    }
    
    if ((msgid = msgget(key, IPC_CREAT | 0600)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    m1 = malloc(14);
    strcpy(m1->text, "123456789");
    
    for (i = 1; i < 11; i++) {
        m1->type = i;
        if (msgsnd(msgid, m1, 9, 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }
    
    free(m1);
    
    m2 = malloc(14);
    m2->type = 0;
    strcpy(m2->text, "aaaaaaaaaa");
    
    if ((k = msgrcv(msgid, m2, 9, 0, 0)) == -1) {
        perror("msgrcv");
        exit(1);
    }
    
    printf("k=%d\n", k);
    printf("m2->type=%d, m2->text='%s'\n", m2->type, m2->text);
    
    free(m2);
    msgctl(msgid, IPC_RMID, 0);
    exit(0);
}