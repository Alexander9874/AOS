// a25a.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    int rc;
    if (argc < 3) {
        fprintf(stderr, "Usage: %s infile command args ...\n", argv[0]);
        exit(1);
    }
    close(0);
    if (open(argv[1], O_RDONLY) == -1) {
        perror(argv[1]);
        exit(1);
    }
    if (fork()) {
        wait (&rc);
        printf ("rc=%d\n", rc);
        exit(0);
    }
    else {
        execvp(argv[2], argv + 2);
        perror(argv[2]);
        exit(1);
    }
}