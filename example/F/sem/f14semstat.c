// f14semstat.c
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
#include <time.h>

int main(int argc, char * argv[], char * envp[]) {
    int semid;
    struct semid_ds m;
    struct passwd * p;
    struct group * g;
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
    
    printf("Key 0x%x\n", m.sem_perm.__key);
    p = getpwuid(m.sem_perm.uid);
    printf("Owner %s\n", p->pw_name);
    g = getgrgid(m.sem_perm.gid);
    printf("Group %s\n", g->gr_name);
    p = getpwuid(m.sem_perm.cuid);
    printf("Creator %s\n", p->pw_name);
    g = getgrgid(m.sem_perm.cgid);
    printf("Creator group %s\n", g->gr_name);
    printf("Permission 0%o\n", m.sem_perm.mode);
    printf("Count of semaphores in set %d\n", m.sem_nsems);
    printf("Time of last semop %s", ctime(&m.sem_otime));
    printf("Time of last change %s", ctime(&m.sem_ctime));
    
    exit(0);
}