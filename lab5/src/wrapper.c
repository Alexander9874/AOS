// wrapper.c
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

extern int errno;

long _read(int __fd, char * __buf, size_t __nbytes) {
    size_t bytes;
    bytes = read(__fd, __buf, __nbytes);
    if(bytes == -1UL) {
        perror("read");
        exit(1);
    }
    return bytes;
}

long _lread(int __fd, char * __buf, size_t __nbytes) {
    size_t bytes, rbytes;
    for (rbytes = 0UL; 
         rbytes != __nbytes;
         rbytes += bytes) {
        bytes = _read(__fd, __buf + rbytes,
                      __nbytes - rbytes);
    }
    return rbytes; 
}

long _write(int __fd, const void *__buf, size_t __nbytes) {
    size_t bytes;
    bytes = write(__fd, __buf, __nbytes);
    if(bytes == -1UL) {
        perror("write");
        exit(1);
    }
    return bytes;
}

long _lwrite(int __fd, const void *__buf, size_t __nbytes) {
    size_t bytes, wbytes;
    for (wbytes = 0UL;
         wbytes != __nbytes;
         wbytes += bytes) {
        bytes = _write(__fd, __buf + wbytes,
                       __nbytes - wbytes);
    }
    return wbytes;
}


int _msgctl (int __msqid, int __cmd, struct msqid_ds *__buf) {
    int ret_code;
    ret_code = msgctl(__msqid, __cmd, __buf);
    if (ret_code == -1) {
        perror("msgctl");
        exit(1);
    }
    return ret_code;
}

key_t _ftok (const char *__pathname, int __proj_id) {
    key_t key;
    key = ftok(__pathname, __proj_id);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    return key;
}

int _msgget (key_t __key, int __msgflg) {
    int msgid;
    msgid = msgget(__key, __msgflg);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    return msgid;
}

int _msgsnd (int __msqid, const void *__msgp, size_t __msgsz, int __msgflg) {
    int ret_code;
    ret_code = msgsnd(__msqid, __msgp, __msgsz, __msgflg);
    if (ret_code == -1) {
        perror("msgsnd");
        exit(1);
    }
    return ret_code;
}

ssize_t _msgrcv (int __msqid, void *__msgp, size_t __msgsz,
		         long int __msgtyp, int __msgflg) {
    ssize_t len = -1;
    while(len == -1) {
        len = msgrcv(__msqid, __msgp, __msgsz, __msgtyp, __msgflg);
        if (len == -1 && errno != EINTR) {
            perror("msgrcv");
            exit(1);
        }
    }
    return len;
}

long _strtol_errmsg(const char * __nptr, int __base,
                    const char * errmsg) {
    long value;
    char * __endptr;
    value = strtol(__nptr, & __endptr, __base);
    if (*__endptr || __nptr == __endptr) {
        fprintf(stderr, "%s: %s\n", __nptr, errmsg);
        exit(1);
    }
    return value;
}