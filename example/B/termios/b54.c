// b54.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

extern int errno;

int main(int argc, char *argv[], char *envp[]) {
    struct termios p;
    char buf[80];
    int l;
    
    if (tcgetattr(0, &p) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    
    p.c_lflag &= ~ECHO;
    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }
    
    write(1, "Input password: ", 16);
    l = read(0, buf, 80);
    write(1, "\n", 1);
    write(1, buf, l);
    
    p.c_lflag |= ECHO;
    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }
    
    exit(0);
}