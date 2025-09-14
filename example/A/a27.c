// a27.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    int l;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }
    if ((l = open(argv[1], O_RDONLY) ) == -1) {
        perror(argv[1]);
        exit(1);
    }
    // Forbid to inherit FD l 
    fcntl(l, F_SETFD, 1);
    if (l != 3)
        dup2(l, 3);
    if (fork()) {
        wait(NULL);
        exit(0);
    }
    else {
        execl("./a27exec.out", "a27exec.out", NULL);
        perror("./a2/exec.out");
        exit(1);
    }
}
