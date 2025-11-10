// a27exec.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buf[4096];
    int l;
    l = read(3, buf, 4096);
    if (l == -1)
        perror("read");
    else
        write(1, buf, l);
    exit(0);
}
