// f17setval.c
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
    int semid, semnum, semval;
    struct semid_ds m;
    char * pp;
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s semid semnum semval\n", argv[0]);
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
    
    semval = strtol(argv[3], &pp, 10);
    if (argv[3] == pp || *pp) {
        fprintf(stderr, "Value semval %s not valid\n", argv[3]);
        exit(1);
    }
    
    if (semctl(semid, 0, IPC_STAT, &m) == -1) {
        perror("semctl");
        exit(1);
    }
    
    if (semctl(semid, semnum, SETVAL, semval) == -1) {
        perror("semctl");
        exit(1);
    }
    
    exit(0);
}