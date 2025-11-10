// b26a.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

extern int errno;

int main()
{
    int a, b, c;
    sigset_t mask;
    a = 0;
    b = 1;
    c = 7;
    sigemptyset(&mask);
    sigaddset(&mask, SIGFPE);
    sigprocmask(SIG_SETMASK, &mask, NULL);
//  c = b / a;
    kill(getpid(), SIGFPE);
    printf("%d\n", c);
    exit(0);
}
