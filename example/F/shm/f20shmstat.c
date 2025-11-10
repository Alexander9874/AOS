// f20shmstat.c
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char * argv[], char * envp[]) {
    int shmid;
    struct shmid_ds m;
    struct passwd * p;
    struct group * g;
    char * pp;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s shmid\n", argv[0]);
        exit(1);
    }
    
    shmid = strtol(argv[1], &pp, 10);
    if (argv[1] == pp || *pp) {
        fprintf(stderr, "Value shmid %s not valid\n", argv[1]);
        exit(1);
    }
    
    if (shmctl(shmid, IPC_STAT, &m) == -1) {
        perror("shmctl");
        exit(1);
    }
    
    printf("Key 0x%x\n", m.shm_perm.__key);
    p = getpwuid(m.shm_perm.uid);
    printf("Owner %s\n", p->pw_name);
    g = getgrgid(m.shm_perm.gid);
    printf("Group %s\n", g->gr_name);
    p = getpwuid(m.shm_perm.cuid);
    printf("Creator %s\n", p->pw_name);
    g = getgrgid(m.shm_perm.cgid);
    printf("Creator group %s\n", g->gr_name);
    printf("Permission %ud\n", m.shm_perm.mode);
    printf("Size of segment %d\n", m.shm_segsz);
    printf("Last attach time %s", ctime(&m.shm_atime));
    printf("Last detach time %s", ctime(&m.shm_dtime));
    printf("Last change time %s", ctime(&m.shm_ctime));
    printf("PID of creator %d\n", m.shm_cpid);
    printf("PID of last operation %d\n", m.shm_lpid);
    printf("Count of process that has attached %d\n", m.shm_nattch);
    
    exit(0);
}