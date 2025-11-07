#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[], char *envp[]) {
    int msgid;
    struct msqid_ds m;
    struct passwd *p;
    struct group *g;
    char *pp;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s msgid\n", argv[0]);
        exit(1);
    }
    
    msgid = strtol(argv[1], &pp, 10);
    if (argv[1] == pp || *pp) {
        fprintf(stderr, "Value msgid %s not valid\n", argv[1]);
        exit(1);
    }
    
    if (msgctl(msgid, IPC_STAT, &m) == -1) {
        perror("msgctl");
        exit(1);
    }
    
    printf("key 0x%x\n", m.msg_perm.__key);
    
    p = getpwuid(m.msg_perm.uid);
    printf("Owner %s\n", p->pw_name);
    
    g = getgrgid(m.msg_perm.gid);
    printf("Group %s\n", g->gr_name);
    
    p = getpwuid(m.msg_perm.cuid);
    printf("Creator %s\n", p->pw_name);
    
    g = getgrgid(m.msg_perm.cgid);
    printf("Creator group %s\n", g->gr_name);
    
    printf("Permission 0%o\n", m.msg_perm.mode);
    printf("Time of last msgsnd %s", ctime(&m.msg_stime));
    printf("Time of last msgrcv %s", ctime(&m.msg_rtime));
    printf("Time of last change %s", ctime(&m.msg_ctime));
    printf("Current number of bytes in queue %lu\n", m.__msg_cbytes);
    printf("Current number of message in queue %lu\n", m.msg_qnum);
    printf("Maximum number of bytes allowed in queue %lu\n", m.msg_qbytes);
    printf("PID of last msgsnd %u\n", m.msg_lspid);
    printf("PID of last msgrcv %u\n", m.msg_lrpid);
    
    exit(0);
}