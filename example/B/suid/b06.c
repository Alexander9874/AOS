//b06.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int uid, euid, suid;
    printf("uid=%d euid=%d\n",
           getuid(), geteuid());
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    exit(0);
}
