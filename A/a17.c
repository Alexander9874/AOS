#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int l;
    char buf[10];
    sprintf(buf, ".tmp%d", getpid());
    close(creat (buf, 0600));
    l=fork();
    if (unlink(buf)!=-1)
        write(1, l?"#":"*", 1);
    exit(0);
}