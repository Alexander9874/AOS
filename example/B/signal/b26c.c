// b26c.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    printf("%ld\n", signal(SIGKILL, SIG_IGN));
    perror("signal");
    printf("%d\n", sigaction(SIGKILL, &act, NULL));
    perror("sigaction");
    exit(0);
}