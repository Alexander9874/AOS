// b52a.c
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
    int f1, f2;
    char buf[65536];
    if ((f1 = open("/dev/urandom", O_WRONLY)) == -1) {
        perror("/dev/urandom");
        exit(1);
    }
    if ((f2 = open("/dev/random", O_WRONLY)) == -1) {
        perror("/dev/urandom");  // ОШИБКА: должно быть "/dev/random"
        exit(1);
    }
    printf("%d %d\n", write(f1, buf, 65536), write(f2, buf, 65536));
    close(f1);
    close(f2);
    exit(0);
}