// wraper.h
#ifndef WRAPER_HEADER
#define WRAPER_HEADER

#define FD_STD_IN  0
#define FD_STD_OUT 1
#define FD_STD_ERR 2

extern void _close(int fd);

extern int _open(const char * const fname, int fmode, ...);

extern void _printf(const char * const fmt, ...);

extern void _fprintf(FILE * stream, const char * const fmt, ...);

extern void rw(const int fin, const int fout);

extern off_t _lseek(int fd, off_t offset, int whence);

extern int _create(const char * const fname, mode_t fmode);

void rw_b(const int fin, const int fout);

#endif // WRAPER_HEADER
