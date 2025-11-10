// b47.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int f;
    char buf[10];
    if ((f = open("/dev/null", O_RDONLY)) == -1) {
        perror("/dev/null");
        exit(1);
    }
    printf("%d\n", read(f, buf, 10));
    close(f);
    exit(0);
}