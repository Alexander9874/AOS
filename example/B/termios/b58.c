// b58.c
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    struct termios p, save;
    char c;
    
    if (tcgetattr(0, &p) == -1) {
        perror("tcgetattr");
        exit(1);
    }

    save = p;
    printf("Noncanonical mode. Escape character ~]\n");
    
    
    p.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    p.c_oflag &= ~OPOST;
    
    p.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    p.c_cflag &= ~(CSIZE | PARENB);
    p.c_cflag |= CS8;

    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }

    p.c_cc[VMIN] = 1;
    setbuf(stdout, NULL);
    
    while (read(0, &c, 1) == 1 && c != 29)
        printf("[%d]", c);
    
    p = save;
    if (tcsetattr(0, TCSANOW, &p) == -1) {
        perror("tcsetattr");
        exit(1);
    }
    
    printf("\n");
    exit(0);
}