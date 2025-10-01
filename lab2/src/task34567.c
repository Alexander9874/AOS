// task34567.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "wraper.h"

int main(int argc, char ** argv) {
    pid_t ret_fork, ret_wait;
    int ret_code;
    int task;
    
    // size_t task_len;
    // char * ptr;

    // if (argc != 2) {
    //     _fprintf(stderr, "Usage: %s task\n",
    //              argv[0]);
    //     exit(1);
    // }

    // task = strtol(argv[1], &ptr, 0);
    // task_len = (size_t)(ptr - argv[1]);
    // if (strlen(argv[1]) != task_len ||
    //            task_len != 1 ||
    //            task < 3 ||
    //            task > 6) {
    //     _fprintf(stderr, "%s: incorrect task\n", argv[1]);
    //     exit(1);
    // }

    if (argc != 1) {
        _fprintf(stderr, "Usage: %s\n",
                 argv[0]);
        exit(1);
    }

    task = (int)(argv[0][6] - '0');
    if (task < 3 || task > 7) {
        _fprintf(stderr, "%s: incorrect task\n", argv[0]);
        exit(1);
    }

    _printf ("I am father before fork.\n"
            "\tMy pid = %d\tppid = %d\tgid = %d\tpgrp = %d\n",
            getpid(), getppid(), getgid(), getpgrp());

    ret_fork = _fork();
    if (ret_fork == 0) {
        if (task == 4)
            sleep(1);
        _printf("I am son.\n"
               "\tMy pid = %d\tppid = %d\tgid = %d\tpgrp = %d\n"
               "\tret_fork = %d\n",
               getpid(), getppid(), getgid(), getpgrp(), ret_fork);
        if (task == 6)
            pause();
        if (task == 7) {
            setpgrp();
            _printf("\tnew son pgrp = %d\n", getpgrp());
            pause();
        }
        exit(123);
    }

    if (task == 3)
        ret_wait= _wait(&ret_code);
    _printf("I am father after fork.\n"
           "\tMy pid = %d\tppid = %d\tgid = %d\tpgrp = %d\n"
           "\tret_fork = %d\n",
           getpid(), getppid(), getgid(), getpgrp(), ret_fork);
    if (task == 3) {
        printf("\tret_wait = %d (shold be same as ret_fork)\n"
               "\tret_code = %x\n"
               "\tIFEXITED = %x\tEXITSTATUS=%x\n"
               "\tIFSIGNALED = %x\tTERMSIG = %x\n",
               ret_wait,
               ret_code,
               WIFEXITED(ret_code), WEXITSTATUS(ret_code),
               WIFSIGNALED(ret_code), WTERMSIG(ret_code));
    }
    if (task == 6 || task == 7)
        pause();


    exit(0);
}
