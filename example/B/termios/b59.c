// b59.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

extern int errno;

int main(int argc, char *argv[], char *envp[]) {
    struct termios p, save;
    int l;
    char c;
    
    if (tcgetattr(0, &p) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    
    save = p;
    
    p.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    p.c_oflag &= ~OPOST;
    p.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    p.c_cflag &= ~(CSIZE | PARENB);
    p.c_cflag |= CS8;
    
    p.c_cc[VTIME] = 5;
    p.c_cc[VMIN] = 0;
    
    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }
    
    printf("%d", l = read(0, &c, 1));
    if (l == 1)
        printf(" [%d]", c);
    
    p = save;
    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }
    
    printf("\n");
    exit(0);
}