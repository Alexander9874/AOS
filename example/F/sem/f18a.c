// f18a.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char * argv[], char * envp[]) {
    key_t key;
    int semid, semnum, sem_op;
    struct sembuf m;
    
    key = ftok(argv[0], 'A');
    semid = semget(key, 5, IPC_CREAT | 0600);
    
    exit(0);
}