// a20.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int i, l;
    for (i = 0;
         i < 1000 && (l = fork()) != -1;
         i++)
        if (!l) exit(0);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    system("ps -eo pid,ppid,stat,comm | grep 'Z'");
    printf("i=%d\n", i);
    exit(0);
}
