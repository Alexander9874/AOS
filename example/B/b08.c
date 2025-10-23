//b08.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

extern int errno;

int main() {
    int uid, euid, suid;
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    execl("./b08a.out", "b08a.out", NULL);
    perror("exec");
    exit(0);
}
