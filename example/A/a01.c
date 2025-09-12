#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    int l, i;
    l=creat("a.txt", 0);
    printf ("l=%d\n", l);
    if (l>=0) {
        i=write(l, "test1\n", 6);
        if (i<0) {
            perror("write");
            printf ("errno=%d\n", errno);
        }
        close (1);
    }
    else{
        perror("creat");
        printf ("errno=%d\n", errno);
        exit(1);
    }
    l=creat("a.txt", 0);
    printf ("l=%d\n", l);
    if (l>=0){
        i=write(1, "“test1\n", 6);
        if (i<0){
            perror ("write");
            printf ("errno=%d\n", errno);
        }
        close (l);
    }
    else{
        perror("creat");
        printf ("errno=%d\n", errno);
    }
    errno=9;
    perror("errno");
    exit(0);
}

