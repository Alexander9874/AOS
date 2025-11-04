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
    int fd;
    char * file_name = "/tmp/task6_lilo";
    char buf[4096] = {0};
    int len;
    int flag = O_RDONLY;

#ifdef NONBLOCK
    flag |= O_NONBLOCK;
    printf("NONBLOCK mode\n");
#endif /* NONBLOCK */
    
    if(mkfifo(file_name, 0777))
        perror("mkfifo");
    
    if((fd = open(file_name, flag)) == -1) {
        perror("open");
        exit(1);
    }

    len = read(fd, buf, 4095);
    if(len < 0) {
        perror("read");
        exit(1);
    }
    if(!len) {
        fprintf(stderr, "read: fifo chanel was closed (or never existed)\n");
        exit(1);
    }

    printf("\tprocess %d by user %d received:\n%s\n", getpid(), geteuid(), buf);
}
