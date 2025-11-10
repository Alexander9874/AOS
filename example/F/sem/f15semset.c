// f15semset.c
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
    int semid, rights;
    struct semid_ds m;
    struct passwd * p;
    struct group * g;
    char * pp;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s semid rights\n", argv[0]);
        exit(1);
    }
    
    semid = strtol(argv[1], &pp, 10);
    if (argv[1] == pp || *pp) {
        fprintf(stderr, "Value semid %s not valid\n", argv[1]);
        exit(1);
    }
    
    rights = strtol(argv[2], &pp, 0);
    if (argv[2] == pp || *pp) {
        fprintf(stderr, "Value rights %s not valid\n", argv[2]);
        exit(1);
    }
    
    m.sem_perm.mode = rights;
    m.sem_perm.uid = getuid();
    m.sem_perm.gid = getgid();
    
    if (semctl(semid, 0, IPC_SET, &m) == -1) {
        perror("semctl");
        exit(1);
    }
    
    exit(0);
}