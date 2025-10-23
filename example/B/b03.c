//b03.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    setenv("PATH", "./c:./a:./b", 1);
    execlp("prog", "prog", NULL);
    perror("exec");
    exit(0);
}
