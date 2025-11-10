// b55.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

extern int errno;

int main(int argc, char *argv[], char *envp[]) {
    struct termios p;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [ 0 | 1 ]\n", argv[0]);
        exit(1);
    }
    
    if (strlen(argv[1]) != 1 || (argv[1][0] != '0' && argv[1][0] != '1')) {
        fprintf(stderr, "Usage: %s [ 0 | 1 ]\n", argv[0]);
        exit(1);
    }
    
    if (tcgetattr(0, &p) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    
    if (argv[1][0] == '1') 
        p.c_lflag |= ISIG;
    else 
        p.c_lflag &= ~ISIG;
    
    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }
    
    exit(0);
}