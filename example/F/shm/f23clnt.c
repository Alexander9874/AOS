// f23clnt.c
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

struct message {
    long mtype;
    int semid;
    int shmid;
    char path[100];
};

void myexit(int semid, int shmid, char * p, int code) {
    semctl(semid, 0, IPC_RMID, 0);
    shmdt(p);
    shmctl(shmid, IPC_RMID, 0);
    exit(code);
}

int main(int argc, char * argv[], char * envp[]) {
    int msgid, semid, shmid;
    struct message m;
    struct sembuf s;
    char * p;
    key_t key;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s server file\n", argv[0]);
        exit(1);
    }
    
    if ((key = ftok(argv[1], 'A')) == -1) {
        perror(argv[0]);
        exit(1);
    }
    
    if ((msgid = msgget(key, 0)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    if ((semid = semget(key, 1, IPC_CREAT | 0600)) == -1) {
        perror("semget");
        exit(1);
    }
    
    if (semctl(semid, 0, SETVAL, 0) == -1) {
        perror("semctl");
        myexit(semid, shmid, p, 1);
    }
    
    if ((shmid = shmget(key, 0xffffff, IPC_CREAT | 0600)) == -1) {
        perror("shmget");
        myexit(semid, shmid, p, 1);
    }
    
    if ((p = shmat(shmid, 0, SHM_RDONLY)) == (void *)-1) {
        perror("shmat");
        myexit(semid, shmid, p, 1);
    }
    
    m.mtype = 1;
    m.semid = semid;
    m.shmid = shmid;
    memset(m.path, 0, 100);
    strncpy(m.path, argv[2], 99);
    
    if (msgsnd(msgid, &m, sizeof(m) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        myexit(semid, shmid, p, 1);
    }
    
    s.sem_num = 0;
    s.sem_op = -1;
    s.sem_flg = 0;
    
    if (semop(semid, &s, 1) == -1) {
        perror("semop");
        myexit(semid, shmid, p, 1);
    }
    
    printf("%s", p);
    myexit(semid, shmid, p, 0);
}