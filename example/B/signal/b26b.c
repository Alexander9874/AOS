// b26b.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

extern int errno;

int main()
{
    struct sigaction act;
    int a, b, c;
    sigset_t mask;
    a = 0;
    b = 1;
    c = 7;
    act.sa_handler=SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaddset(&mask, SIGFPE);
    printf("%d\n", sigaction(SIGFPE, &act, NULL));
    c = b / a;
//    kill(getpid(), SIGFPE);
    printf("%d\n", c);
    exit(0);
}
