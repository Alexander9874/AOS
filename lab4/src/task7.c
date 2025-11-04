#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>

#ifdef LOCK_BOTH
#define LOCK_FATHER
#define LOCK_SON
#endif /* LOCK_BOTH */

int main(int argc, char ** argv) {
    int fd;
    char * file_name;
    char buf[4096] = {0};
    int len;

    struct flock lock;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    file_name = argv[1];

    if(fork())
    {
        // father
        int val = 0x0a746163; // "cat\n"

        if((fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
            perror("open father");
            exit(1);
        }

#ifdef LOCK_FATHER
        lock.l_type = F_WRLCK;
        if(fcntl(fd, F_SETLKW, &lock)) {
            perror("fcntl");
            exit(1);
        }
#endif /* LOCK_FATHER */

        for(int i = 0; i < 64; i++) {
            write(fd, & val, sizeof(int));
        }

#ifdef LOCK_FATHER
        lock.l_type = F_UNLCK;
        if(fcntl(fd, F_SETLKW, &lock)) {
            perror("fcntl");
            exit(1);
        }
#endif /* LOCK_FATHER */

        close(fd);
        exit(0);        
    }
    // son
    if((fd = open(file_name, O_RDWR)) == -1) {
        perror("open son");
        exit(1);
    }

#ifdef LOCK_SON
    lock.l_type = F_WRLCK;
    if(fcntl(fd, F_SETLKW, &lock)) {
        perror("fcntl");
        exit(1);
    }
#endif /* LOCK_SON */

    while(1) {
        len = read(fd, buf, 4096);
        if(len < 0) {
            perror("read");
            exit(1);
        }
        if(!len) {
            break;
        }
        
        if(write(STDOUT_FILENO, buf, len) < 0) {
            perror("write");
            exit(1);
        }
    }

#ifdef LOCK_SON
    lock.l_type = F_UNLCK;
    if(fcntl(fd, F_SETLKW, &lock)) {
        perror("fcntl 2");
        exit(1);
    }
#endif /* LOCK_SON */
    
    close(fd);
    exit(0);
}
