//b04.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    setenv("PATH", "b", 1);
    execp("prog", "prog", NULL);
    perror("exec");
    exit(0);
}
