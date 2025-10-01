// task89.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "wraper.h"

int main(int argc, char ** argv) {
    int task;
    int fin, fout;
    pid_t ret_fork;

    if (argc != 4 && argc != 2) {
        _fprintf(stderr,
                 "Usage: %s input output_father output_son\n"
                 "\t(output - optional)\n",
                 argv[0]);
        exit(1);
    }

    task = (int)(argv[0][6] - '0');
    if (task < 8 || task > 9) {
        _fprintf(stderr, "%s: incorrect task\n", argv[0]);
        exit(1);
    }

    if (task == 9)
        ret_fork = _fork();

    fin = _open(argv[1], O_RDONLY);

    if (task == 8)
        ret_fork = _fork();

    fout = argc == 4 ? _open(argv[2 + !ret_fork], O_WRONLY | O_CREAT | O_TRUNC , 0600)
         : FD_STD_OUT;
    rw_small_buf(fin, fout);

    exit(0);
}
