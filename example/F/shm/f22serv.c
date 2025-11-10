// f22serv.c
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

struct message {
    long mtype;
    int semid;
    int shmid;
    char path[108];
};

int flag = 1;

void handler(int sig) {
    flag = 0;
}

int main(int argc, char * argv[], char * envp[]) {
    int msgid, f;
    struct message m;
    struct stat st;
    struct sembuf s;
    char * p;
    key_t key;
    
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);
    signal(SIGHUP, handler);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGALRM, handler);
    
    if ((key = ftok(argv[0], 'A')) == -1) {
        perror(argv[0]);
        exit(1);
    }
    
    if ((msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    while (flag) {
        if (msgrcv(msgid, &m, sizeof(m) - sizeof(long), 0, 0) == -1) {
            continue;
        } else {
            if ((p = shmat(m.shmid, 0, 0)) == (void *)-1) {
                continue;
            }
            
            if (stat(m.path, &st) == 0 && (f = open(m.path, O_RDONLY)) != -1 && read(f, p, st.st_size) != -1) {
                p[st.st_size] = '\0';
                shmdt(p);
                close(f);
            } else {
                strcpy(p, "ERROR\n");
            }
            
            s.sem_num = 0;
            s.sem_op = 1;
            s.sem_flg = 0;
            semop(m.semid, &s, 1);
        }
    }
    
    msgctl(msgid, IPC_RMID, 0);
    exit(0);
}