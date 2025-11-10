// b26aa.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

extern int errno;

void handler(int sig) {}

int main()
{
    int a, b, c;
    a = 0;
    b = 1;
    c = 7;
    signal(SIGFPE, handler);
    c = b / a;
    printf("%d\n", c);
    exit(0);
}
