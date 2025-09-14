// a22.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char * argv[]) {
    int f, l;
    char c;
    struct timespec t;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }
    if ((f = open(argv[1], O_RDONLY) ) == -1) {
        perror(argv[1]);
        exit(1);
    }
    fork();
    while ((l = read(f, &c, 1)) > 0) {
        t.tv_sec = 0;
        t.tv_nsec = 100000;
        nanosleep(&t, NULL);
        write(1, &c, l);
    }
    close(f);
    exit(0);
}
