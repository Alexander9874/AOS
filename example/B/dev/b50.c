// b50.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int f;
    if ((f = open("/dev/full", O_WRONLY)) == -1) {
        perror("/dev/full");
        exit(1);
    }
    printf("%d\n", write(f, "", 1));
    perror("write");
    close(f);
    exit(0);
}