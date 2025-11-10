// f21shmset.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

int main(int argc, char * argv[], char * envp[]) {
    int shmid, rights;
    struct shmid_ds m;
    struct passwd * p;
    struct group * g;
    char * pp;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s shmid rights\n", argv[0]);
        exit(1);
    }
    
    shmid = strtol(argv[1], &pp, 10);
    if (argv[1] == pp || *pp) {
        fprintf(stderr, "Value shmid %s not valid\n", argv[1]);
        exit(1);
    }
    
    rights = strtol(argv[2], &pp, 0);
    if (argv[2] == pp || *pp) {
        fprintf(stderr, "Value rights %s not valid\n", argv[2]);
        exit(1);
    }
    
    if (shmctl(shmid, IPC_STAT, &m) == -1) {
        perror("shmctl IPC_STAT");
        exit(1);
    }
    
    m.shm_perm.mode = rights;
    m.shm_perm.uid = getuid();
    m.shm_perm.gid = getgid();
    
    if (shmctl(shmid, IPC_SET, &m) == -1) {
        perror("shmctl IPC_SET");
        exit(1);
    }
    
    exit(0);
}