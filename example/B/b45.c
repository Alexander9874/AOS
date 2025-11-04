// b45.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    struct flock lock;
    int f, l, c;
    if ((f=open("count", O_RDWR | O_CREAT, 0600))==-1){
        perror("count");
        exit(1);
    }
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    lock.l_type=F_WRLCK;
    // if (fcntl(f, F_SETLKW, &lock)==-1){
    //     perror("count");
    //     exit(1);
    // }
    if ((l=read(f, &c, 4))==-1){
        perror("count");
        exit(1);
    }
    if (l!=4){
        ftruncate(f, 0);
        c=0;
    }
    c++;
    lseek(f, 0, SEEK_SET);
    write(f, &c, 4);
    lock.l_type=F_UNLCK;
    // fcntl(f, F_SETLK, &lock);
    close(f);
    exit(0);
}