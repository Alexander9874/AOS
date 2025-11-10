// b51.c
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
    int f, i, l;
    char buf[1000000];
    if ((f = open("/dev/random", O_RDONLY)) == -1) {
        perror("/dev/random");
        exit(1);
    }
    if ((l = read(f, buf, 1000000)) == -1) {
        perror("read");
        exit(1);
    }
    for (i = 0; i < 1; i++)
        printf("%d ", buf[i]);
    printf("\n");
    printf("l=%d\n", l);
    close(f);
    exit(0);
}