// f19semopundo.c
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
    int semid, semnum, sem_op;
    struct sembuf m;
    char * pp;
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s semid semnum semop\n", argv[0]);
        exit(1);
    }
    
    semid = strtol(argv[1], &pp, 10);
    if (argv[1] == pp || *pp) {
        fprintf(stderr, "Value semid %s not valid\n", argv[1]);
        exit(1);
    }
    
    semnum = strtol(argv[2], &pp, 10);
    if (argv[2] == pp || *pp) {
        fprintf(stderr, "Value semnum %s not valid\n", argv[2]);
        exit(1);
    }
    
    sem_op = strtol(argv[3], &pp, 10);
    if (argv[3] == pp || *pp) {
        fprintf(stderr, "Value semop %s not valid\n", argv[3]);
        exit(1);
    }
    
    m.sem_num = semnum;
    m.sem_op = sem_op;
    m.sem_flg = SEM_UNDO;
    
    if (semop(semid, &m, 1) == -1) {
        perror("semop");
        exit(1);
    }
    
    sleep(30);
    exit(0);
}