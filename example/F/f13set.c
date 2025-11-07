#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    int msgid, rights;
    struct msqid_ds m;
    struct passwd *p;
    struct group *g;
    char *pp;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s msgid rights\n", argv[0]);
        exit(1);
    }
    
    msgid = strtol(argv[1], &pp, 10);
    if (argv[1] == pp || *pp) {
        fprintf(stderr, "Value msgid %s not valid\n", argv[1]);
        exit(1);
    }
    
    rights = strtol(argv[2], &pp, 0);
    if (argv[2] == pp || *pp) {
        fprintf(stderr, "Value rights %s not valid\n", argv[2]);
        exit(1);
    }
    
    if (msgctl(msgid, IPC_STAT, &m) == -1) {
        perror("msgctl IPC_STAT");
        exit(1);
    }
    
    m.msg_perm.mode = rights;
    m.msg_perm.uid = getuid();
    m.msg_perm.gid = getgid();
    m.msg_qbytes = 16384;
    
    if (msgctl(msgid, IPC_SET, &m) == -1) {
        perror("msgctl IPC_SET");
        exit(1);
    }
    
    exit(0);
}