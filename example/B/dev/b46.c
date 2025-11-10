// b46.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int f, i;
    char buf[65536];
    if ((f = open("/dev/zero", O_RDONLY)) == -1) { 
        perror("/dev/zero"); 
        exit(1); 
    }
    if (read(f, buf, 65536) != 65536) { 
        perror("read"); 
        exit(1); 
    } 
    for (i = 0; i < 10; i++) 
        printf("%d ", buf[i]); 
    for (i = 0; i < 65536; i++) 
        if (buf[i] != 0) 
            printf("Error %d\n", i); 
    printf("\n"); 
    close(f); 
    exit(0); 
}