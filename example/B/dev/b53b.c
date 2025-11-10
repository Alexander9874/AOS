// b53b.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>  // Исправлено: было umistd.h

extern int errno;

int main(int argc, char *argv[], char *envp[]) {
    int f1, f2, l;  // Исправлено: было 1 (единица)
    char buf[4096];
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        exit(1);
    }
    
    if ((f1 = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(1);
    }
    
    if ((f2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0600)) == -1) {
        perror(argv[2]);
        exit(1);
    }
    
    while ((l = read(f1, buf, 4096)) > 0)  // Исправлено: было 1
        write(f2, buf, l);  // Исправлено: было 1
    
    close(f1);
    close(f2);
    exit(0);
}