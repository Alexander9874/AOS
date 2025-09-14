// a26.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int rc;
    char * env[] = { "A=1", "B=2", "C=3", NULL };
    if (fork()) {
        wait(NULL);
        exit(0);
    }
    else{
        execle("/usr/bin/env", "env", NULL, env);
        perror("/usr/bin/env");
        exit(1);
    }
}
