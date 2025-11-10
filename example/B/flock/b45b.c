// b45b.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    struct flock lock;
    int f, l;
    char * prefix;
    char buf[80];
    if ((f=open("count", O_RDWR | O_CREAT, 0600))==-1) {
        perror("count");
        exit(1);
    }
    if (fork()) 
        prefix="father: ";
    else 
        prefix="son: ";
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    if (fcntl(f, F_SETLKW, &lock)==-1) {
        perror("count");
        exit(1);
    }
    while ((l=read(0, buf, 79))>0){
        buf[l]='\0'; 
        printf("%s%s", prefix, buf);
        sleep(1);
    }
    lock.l_type=F_UNLCK;
    fcntl(f, F_SETLK, &lock);
    close(f);
    wait(NULL);
    exit(0);
}