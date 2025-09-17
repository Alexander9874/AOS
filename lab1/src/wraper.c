// wraper.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>

void _close(int fd) {
    if (close(fd)) {
        perror("close");
        exit(1);
    }
    return;
}

int _open(const char * const fname, int fmode, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmode);
    int fd = open(fname, fmode, arg_ptr);
    va_end(arg_ptr);
    if (fd  == -1) {
        perror("open");
        exit(1);
    }
    return fd;
}

void _printf(const char * const fmt, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    int ret_code = vprintf(fmt, arg_ptr);
    va_end(arg_ptr);
    if (ret_code < 0) {
        perror("printf");
        exit(1);
    }
    return;
}

void _fprintf(FILE * stream, const char * const fmt, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    int ret_code = vfprintf(stream, fmt, arg_ptr);
    va_end(arg_ptr);
    if (ret_code < 0) {
        perror("fprintf");
        exit(1);
    }
    return;
}

void rw(const int fin, const int fout) {
    int ret_code;
    char buf[4096];
    while ((ret_code = read(fin, buf, 4096)) > 0) {
        ret_code = write(fout, buf, ret_code);
        if (ret_code == -1) {
            perror("write");
            exit(1);
        }
    }
    if (ret_code == -1) {
        perror("read");
        exit(1);
    }
    return;
}

off_t _lseek(int fd, off_t offset, int whence) {
    off_t ret_offset;
    if((ret_offset = lseek(fd, offset, whence)) < 0) {
        perror("lseek");
        exit(0);
    }
    return ret_offset;  
}

void rw_b(const int fin, const int fout) {
    char c;
    char buf[4096];
    int i = 0;
    for (lseek(fin, 1, SEEK_END);
         lseek(fin, -2, SEEK_CUR) != -1;) {
        if (read(fin, &c, 1) == -1) {
            perror("read");
            exit(0);
        }
        if (i == 4096) {
            if (write(fout, buf, 4096) == -1) {
                perror("write");
                exit(0);
            }
            i = 0;
        }
        buf[i++] = c;
    }
    if (write(fout, buf, i) == -1) {
        perror("write");
        exit(0);
    }
    return;
}        

int _create(const char * const fname, mode_t fmode) {
    int fd = creat(fname, fmode);
    if (fd == -1){
        perror("creat");
        exit(1);
    }
    return fd;
}
