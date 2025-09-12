#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int l;
    printf ("My nice is %d\n", nice(0));
    nice (5);
    printf ("My nice is %d\n", nice(0));
    nice (5);
    printf ("My nice is %d\n", nice(0));
    nice(5);
    printf ("My nice is %d\n", nice(0));
    nice(5);
    printf("My nice is %d\n", nice(0));
    l=nice(-5);
    if (1==-1) perror("nice");
    printf("My nice is %d\n", nice(0));
    exit(0);
}









