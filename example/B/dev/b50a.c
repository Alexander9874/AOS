// b50a.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>  // Исправлено: было fontl.h
#include <unistd.h>

extern int errno;

int main() {
    int i, f;
    char buf[65536];
    if ((f = open("/dev/full", O_RDONLY)) == -1) {
        perror("/dev/full");
        exit(1);
    }
    printf("%d\n", read(f, buf, 65536));
    for (i = 0; i < 65536; i++)
        if (buf[i] != '\0')  // Исправлено: было :=
            printf("Error\n");
    close(f);
    printf("%d\n", BUFSIZ);
    exit(0);
}