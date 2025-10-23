//b07.c
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

extern int errno;

int main() {
    uid_t uid, euid, suid;
    printf("uid=%d euid=%d\n", getuid(), geteuid());
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    
    if (fork()) {
        wait(NULL);
    } else {
        getresuid(&uid, &euid, &suid);
        printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    }
    
    exit(0);
}
