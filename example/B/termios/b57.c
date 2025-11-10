// b57.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

extern int errno;

int main(int argc, char *argv[], char *envp[]) {
    struct termios p, save;
    char buf[100];
    int l;
    
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
    
    save = p;
    
    if (argv[1][0] == '1') {
        p.c_cc[VINTR] = 'A';
        p.c_cc[VQUIT] = 'B';
        p.c_cc[VSUSP] = 'C';
    } else {
        p.c_cc[VINTR] = '\003';
        p.c_cc[VQUIT] = '\034';
        p.c_cc[VSUSP] = '\032';
    }
    
    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }
    
    exit(0);
}