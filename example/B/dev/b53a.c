// b53a.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

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
    
    if ((f1 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0600)) == -1) {
        perror(argv[1]);
        exit(1);
    }
    
    if (lseek(f1, l - 1, SEEK_SET) == -1) {
        perror("lseek");
        exit(1);
    }
    
    if (write(f1, "", 1) == -1) {
        perror("write");
        exit(1);
    }
    
    if ((f2 = open("/dev/urandom", O_RDONLY)) == -1) {
        perror("/dev/urandom");
        exit(1);
    }
    
    if ((p = mmap(NULL, l, PROT_WRITE | PROT_READ, MAP_SHARED, f1, 0)) == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    close(f1);
    
    if ((k = read(f2, p, l)) == -1) {
        perror("read");
        exit(1);
    }
    
    if (l != k)
        fprintf(stderr, "Only %d first bytes in file correct", k);
    
    if (munmap(p, l) == -1) {
        perror("munmap");
        exit(1);
    }
    
    close(f2);
    exit(0);
}