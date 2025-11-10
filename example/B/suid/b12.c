//b12.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

extern int errno;

int main() {
    int uid, euid, suid, myuid;
    getresuid(&uid, &euid, &suid);
    myuid = uid;
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    
    setreuid(euid, euid);
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
    
    setreuid(myuid, myuid);
    getresuid(&uid, &euid, &suid);
    printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);

    exit(0);
}
