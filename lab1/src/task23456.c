// task2345.c
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "menu.h"
#include "wrapper.h"

int main(int argc, char ** argv) {
    int fd;
    int task = 2;
    long fmode;
    size_t fmode_len;
    size_t task_len;
    char * ptr;

    if (argc < 3 || argc > 4) {
        _fprintf(stderr, "Usage: %s file filemode task(optinal)\n",
                 argv[0]);
        exit(1);
    }

    fmode = strtol(argv[2], &ptr, 8);
    fmode_len = (size_t)(ptr - argv[2]);
    if (strlen(argv[2]) != fmode_len || fmode_len > 4) {
        _fprintf(stderr, "%s: incorrect filemode\n", argv[2]);
        exit(1);
    }

    if (argc == 4) {
        task = strtol(argv[3], &ptr, 0);
        task_len = (size_t)(ptr - argv[3]);
        if (strlen(argv[3]) != task_len || task_len != 1) {
            _fprintf(stderr, "%s: incorrect task\n", argv[3]);
            exit(1);
        }
    }
// TASK 4 & 5 & 6
    if (task == 4 || task == 5 || task == 6) {
        _printf("TASK 4 & 5 & 6\n");
        fd = _open(argv[1], O_RDWR | O_CREAT, fmode);
        menu_Main(fd);
        exit(0);
    }

// TASK 2 & 3
    // PART 1
    _printf("TASK 2 & 3\tPART 1\n");
    fd = _create(argv[1], fmode);
    rw(0, fd);
    _close(fd);

    // PART 2
    _printf("TASK 2 & 3\tPART 2\n");
    fd = _open(argv[1], O_RDONLY);
    rw(fd, 1);
    _close(fd);

    // PART 3
    _printf("TASK 2 & 3\tPART 3\n");
    fd = _open(argv[1], O_RDWR);
    _close(fd);
    exit(0);
}
