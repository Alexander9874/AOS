// task10.c
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "wraper.h"

int main(int argc, char ** argv) {
    int fout, fin;
    if (argc > 3) {
        _fprintf(stderr, "Usage: %s file_input file_output\n",
                 argv[0]);
        exit(1);
    }
    fout = argc == 3 ? _open(argv[2], O_RDWR | O_CREAT, 0600)
         : FD_STD_OUT;
    fin  = argc >= 2 ? _open(argv[1], O_RDWR | O_CREAT, 0600)
         : FD_STD_IN;
    rw_b(fin, fout);
    _close(fin);
    _close(fout);
    exit(0);
}
