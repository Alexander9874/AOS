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
    FILE * file;
    int flag = O_WRONLY;

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
    file = fdopen(fd, "w");

    fprintf(file, "process %d by user %d sends its regards.\n", getpid(), geteuid());

    fclose(file);
    exit(0);
}
