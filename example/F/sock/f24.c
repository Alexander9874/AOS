// f24.c
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[], char * envp[]) {
    int sv[2], l;
    char buf[80];
    
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        exit(1);
    }
    
    if (fork()) {
        close(sv[1]);
        printf("I am father\n");
        printf("I send message to son\n");
        write(sv[0], "Hello, son!\n", 12);
        printf("I wait for message from son\n");
        l = read(sv[0], buf, 80);
        if (l > 0) {
            buf[l] = '\0';
            printf("%s", buf);
        }
        else {
            fprintf(stderr, "Read from socket failed\n");
        }
        close(sv[0]);
        wait(NULL);
        exit(0);
    }
    else {
        close(sv[0]);
        printf("I am son\n");
        printf("I wait for message from father\n");
        l = read(sv[1], buf, 80);
        if (l > 0) {
            buf[l] = '\0';
            printf("%s", buf);
        }
        else {
            fprintf(stderr, "Read from socket failed\n");
        }
        printf("I send message to father\n");
        write(sv[1], "Hello, father\n", 15);
        close(sv[1]);
        exit(0);
    }
}