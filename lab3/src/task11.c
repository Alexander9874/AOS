#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <time.h>


void syncronize(pid_t other_pid) {
    pid_t this_pid = getpid();

    int counter = 0;
    
    sigset_t mask1, mask2;
    sigemptyset(&mask1);
    sigemptyset(&mask2);

    sigaddset(&mask1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask1, NULL);

    if (this_pid < other_pid)
        sigsuspend(&mask2);

    while(1) {
        printf("%d\t%d\n", this_pid, counter++);
        sleep(2);
        kill(other_pid, SIGUSR1);
        sigsuspend(&mask2);
    }
}

void disp(int sig) {
    return;
}

int main() {
    struct sigaction act;
    act.sa_handler = disp;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);

    int father, pid;
    father = getpid();

    if ((pid = fork()) == 0)
        syncronize(father);
    else
        syncronize(pid);

    return 0;
}
