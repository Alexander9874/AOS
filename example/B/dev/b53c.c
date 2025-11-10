// b53c.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    int f1, f2;
    char *p1, *p2;
    struct stat st;

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

    if (fstat(f1, &st) == -1) {
        perror(argv[1]);
        exit(1);
    }

    if ((p1 = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, f1, 0)) == MAP_FAILED) {
        perror(argv[1]);
        exit(1);
    }

    if (lseek(f2, st.st_size - 1, SEEK_SET) == -1) {
        perror(argv[2]);
        exit(1);
    }

    if (write(f2, "", 1) == -1) {
        perror(argv[2]);
        exit(1);
    }

    if ((p2 = mmap(NULL, st.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, f2, 0)) == MAP_FAILED) {
        perror(argv[2]);
        exit(1);
    }

    close(f1);
    close(f2);

    memcpy(p2, p1, st.st_size);

    if (munmap(p1, st.st_size) == -1) {
        perror(argv[1]);
        exit(1);
    }

    if (munmap(p2, st.st_size) == -1) {
        perror(argv[2]);
        exit(1);
    }

    return 0;
}