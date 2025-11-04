// task1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <time.h>

int main() {
    int fd1[2], fd2[2];
    char buf[4096] = {0};
    int len;

    pipe(fd1);
    pipe(fd2);

    if (fork() == 0) {
        close(fd1[1]);
        close(fd2[0]);

        len = read(fd1[0], buf, 4096);
        if(len < 0) {
            perror("read_35");
            exit(1);
        }
        write(1, buf, len);
        write(fd2[1], "Hello, father! I am son!\n", 25);
        
        len = read(fd1[0], buf, 4096);
        if(len < 0) {
            perror("read_43");
            exit(1);
        }
        write(1, buf, len);
        write(fd2[1], "Goodbuy, father! I was son!\n", 28);

        close(fd1[0]);
        close(fd2[1]);
        
    } else {
        close(fd1[0]);
        close(fd2[1]);

        write(fd1[1], "Hello, son! I am father!\n", 25);
        len = read(fd2[0], buf, 4096);
        if(len < 0) {
            perror("read_59");
            exit(1);
        }
        write(1, buf, len);
        
        write(fd1[1], "Goodbuy, son! I was father!\n", 28);
        len = read(fd2[0], buf, 4096);
        if(len < 0) {
            perror("read_67");
            exit(1);
        }
        write(1, buf, len);

        close(fd1[1]);
        close(fd2[0]);       
    }
    exit(0);
}
