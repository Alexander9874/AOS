//b11.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

extern int errno;

int main() {
    int uid, euid, suduid;
    getresuid(&uid, &euid, &suduid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suduid);
    setresuid(euid, euid);
    getresuid(&uid, &euid, &suduid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suduid);
    execl("./b08a.out", "./b08a.out", NULL);
    perror("exec");
    exit(0);
}
