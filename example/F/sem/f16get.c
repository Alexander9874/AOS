// f16get.c
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
    int semid, i;
    unsigned short * mas;
    struct semid_ds m;
    char * pp;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s semid\n", argv[0]);
        exit(1);
    }
    
    semid = strtol(argv[1], &pp, 10);
    if (argv[1] == pp || *pp) {
        fprintf(stderr, "Value semid %s not valid\n", argv[1]);
        exit(1);
    }
    
    if (semctl(semid, 0, IPC_STAT, &m) == -1) {
        perror("semctl");
        exit(1);
    }
    
    printf("GETALL - values of all semaphores\n");
    mas = malloc(sizeof(short) * m.sem_nsems);
    if (mas == NULL) {
        perror("Cannot allocate memory\n");
        exit(1);
    }
    
    if (semctl(semid, 0, GETALL, mas) == -1) {
        perror("semctl");
        exit(1);
    }
    
    for (i = 0; i < m.sem_nsems; i++)
        printf("%d ", mas[i]);
    printf("\n");
    free(mas);
    
    printf("GETVAL - values of each semaphore\n");
    for (i = 0; i < m.sem_nsems; i++)
        printf("%d ", semctl(semid, i, GETVAL));
    printf("\n");
    
    printf("GETNCNT - count of process waiting for an increase of each semaphore\n");
    for (i = 0; i < m.sem_nsems; i++)
        printf("%d ", semctl(semid, i, GETNCNT));
    printf("\n");
    
    printf("GETZCNT - count of process waiting for zero of each semaphore\n");
    for (i = 0; i < m.sem_nsems; i++)
        printf("%d ", semctl(semid, i, GETZCNT));
    printf("\n");
    
    printf("GETPID - PID of process that executed last semop of each semaphore\n");
    for (i = 0; i < m.sem_nsems; i++)
        printf("%d ", semctl(semid, i, GETPID));
    printf("\n");
    
    exit(0);
}