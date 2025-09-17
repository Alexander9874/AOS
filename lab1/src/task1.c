// task1.c
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <stdlib.h>

#include <stddef.h>
#include <stdarg.h>

#define _printf(...) \
    if (printf(__VA_ARGS__) < 0) { \
        perror("printf"); \
        exit(1); \
    } \

int main(int argc, char * argv[]) {
    int ret_code;
    if (getpid() % 2 || getuid() == 1)
        ret_code = open(argv[0], 0);
    else
        ret_code = creat("/new_file", 0);
    _printf("\nret_code = %d\n", ret_code);
    if(ret_code) {
        perror("create");
        _printf("errno = %d\n", errno);
        _printf("error: %s\n", strerror(errno));

        // sys_errlist - undefined
        // printf("error: %s\n", sys_errlist[errno]);
    }
    exit(0);
}