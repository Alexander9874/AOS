// b53.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main(int argc, char *argv[], char *envp[]) {
    int f1, f2, l, k;
    char *p, buf[4096];
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s file size\n", argv[0]);
        exit(1);
    }
    
    l = strtol(argv[2], &p, 10);
    if (strlen(argv[2]) == 0 || strlen(argv[2]) != p - argv[2] || errno == ERANGE) {
        fprintf(stderr, "Usage: %s not correct size\n", argv[2]);
        exit(1);
    }
    
    if ((f1 = creat(argv[1], 0600)) == -1) {
        perror(argv[1]);
        exit(1);
    }
    
    if ((f2 = open("/dev/urandom", O_RDONLY)) == -1) {
        perror("/dev/urandom");
        exit(1);
    }
    
    for (; l > 0; l -= k) {
        if ((k = read(f2, buf, l < 4096 ? l : 4096)) > 0) {
            write(f1, buf, k);
        } else {
            break;
        }
    }
    
    close(f1);
    close(f2);
    exit(0);
}