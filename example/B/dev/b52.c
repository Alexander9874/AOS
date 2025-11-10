// b52.c
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>  // Добавлено для exit()
#include <stdio.h>   // Добавлено для printf()

extern int errno;

int main() {
    int f, i, l, ipos, ineg, izero;
    char buf[1000000];
    if ((f = open("/dev/urandom", O_RDONLY)) == -1) { 
        perror("/dev/urandom"); 
        exit(1);
    }
    if ((l = read(f, buf, 1000000)) == -1) {  // Исправлено: l вместо 1
        perror("read");
        exit(1);
    }
    for (i = ipos = ineg = izero = 0; i < l; i++) {  // Исправлено: i < l вместо i < 1
        if (buf[i] > 0) ipos++;
        if (buf[i] < 0) ineg++;
        if (buf[i] == 0) izero++;
        printf("%d ", buf[i]);
    }
    printf("\n");
    printf("l=%d ipos=%d ineg=%d izero=%d\n", l, ipos, ineg, izero);  // Исправлено
    close(f);
    exit(0);
}