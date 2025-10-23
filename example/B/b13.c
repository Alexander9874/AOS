//b13.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

extern int errno;

int main() {
    int uid, euid, suid;
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    setuid(uid);
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    setuid(suid);
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    exit(0);
}
