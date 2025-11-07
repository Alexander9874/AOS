// warapper.h
#ifndef WRAPPER_HEADER
#define WRAPPER_HEADER

#include <sys/ipc.h>
#include <sys/msg.h>

extern long _read(int __fd, char * __buf, long __nbytes);

extern long _lread(int __fd, char * __buf, long __nbytes);

extern long _write(int __fd, const void *__buf, size_t __nbytes);

extern long _lwrite(int __fd, const void *__buf, size_t __nbytes);

extern int _msgctl(int __msqid, int __cmd, struct msqid_ds *__buf);

extern key_t _ftok (const char *__pathname, int __proj_id);

extern int _msgget (key_t __key, int __msgflg);

extern int _msgsnd (int __msqid, const void *__msgp, size_t __msgsz, int __msgflg);

extern ssize_t _msgrcv (int __msqid, void *__msgp, size_t __msgsz,
		                long int __msgtyp, int __msgflg);

extern long _strtol_errmsg(const char * __nptr, int __base,
                           const char * errmsg);

#endif /* WRAPPER_HEADER */