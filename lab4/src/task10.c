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

int main() {
    char buf[4096] = {0};
    int len;
    char * prefix;
    int is_father;

    is_father = fork();
    
    if(is_father)
        prefix = "father";
    else 
        prefix = "son";

#ifdef LOCK_STDIN

    struct flock lock;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;

    lock.l_type = F_WRLCK;
    if(fcntl(0, F_SETLKW, &lock)) {
        perror("fcntl");
        exit(1);
    }

    printf("%s locked stream\n", prefix);
#endif /* LOCK_STDIN */


    while(1) {
        len = read(STDIN_FILENO, buf, 4095);
        if(len < 0) {
            perror("read");
            exit(1);
        }
        if(!len) {
            break;
        }
        buf[len] = '\0';
        printf("%s : %s", prefix, buf);
    }

#ifdef LOCK_STDIN
    lock.l_type = F_UNLCK;
    if(fcntl(0, F_SETLKW, &lock)) {
        perror("fcntl");
        exit(1);
    }
#endif /* LOCK_STDIN */

    if(is_father)
        wait(NULL);

    exit(0);
}
