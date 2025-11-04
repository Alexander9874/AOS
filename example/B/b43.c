#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

extern int errno;

int main(int argc, char * argv[], char * envp[]) {
    struct stat st;
    int f, l;
    char buf[4096];
    if (argc!=2) {
        fprintf(stderr, "Usage: %s fifofile\n", argv[0]);
        exit(1);
    }
    if (stat(argv[1], &st)==-1) {
        perror(argv[1]);
        exit(1);
    }
    if (!S_ISFIFO(st.st_mode)) {
        fprintf(stderr, "%s not fifofile\n", argv[1]);
        exit(1);
    }
    printf("Before open\n");
    if ((f=open(argv[1], O_WRONLY))==-1) {
        perror(argv[1]);
        exit(1);
    }
    printf("After open\n");
    while ((l=read(0, buf, 4096))>0)
        write(f, buf, l);
    close(f);
    exit(0);
}