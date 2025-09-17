// task7.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void stat_Print(struct stat statbuf) {
    printf( "                          Device %lu\n"
            "              File serial number %lu\n"
            "                       File mode %u\n"
            "                      Link count %lu\n"
            "     User ID of the file's owner %ud\n"
            "    Group ID of the file's group %ud\n"
            "        Device number, if device %lu\n"
            "          Size of file, in bytes %ld\n"
            "      Optimal block size for I/O %ld\n"
            "Number 512-byte blocks allocated %ld\n"
            "             Time of last access %ld\n"
         // "           Nscecs of last access %lu\n"
            "       Time of last modification %lu\n"
         // "      Nsecs of last modification %lu\n"
            "      Time of last status change %lu\n",
         // "     Nsecs of last status change %lu\n"
            statbuf.st_dev,
            statbuf.st_ino,
            statbuf.st_mode,
            statbuf.st_nlink,
            statbuf.st_uid,
            statbuf.st_gid,
            statbuf.st_rdev,
            statbuf.st_size,
            statbuf.st_blksize,
            statbuf.st_blocks,
            statbuf.st_atime,
         // statbuf.st_atimensec,
            statbuf.st_mtime,
         // statbuf.st_mtimensec,
            statbuf.st_ctime
         // statbuf.st_ctimensec
            );
    return;
}

int main(int argc, char *argv[]) {
    struct stat statbuf;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }
    stat(argv[1], &statbuf);
    stat_Print(statbuf);
    exit(0);
}

    // __dev_t st_dev;		/* Device.  */
    // __ino_t st_ino;		/* File serial number.	*/
    // __mode_t st_mode;			/* File mode.  */
    // __nlink_t st_nlink;			/* Link count.  */
    // __uid_t st_uid;		/* User ID of the file's owner.	*/
    // __gid_t st_gid;		/* Group ID of the file's group.*/
    // __dev_t st_rdev;		/* Device number, if device.  */
    // __off_t st_size;			/* Size of file, in bytes.  */
    // __blksize_t st_blksize;	/* Optimal block size for I/O.  */
    // __blkcnt_t st_blocks;		/* Number 512-byte blocks allocated. */
    // __time_t st_atime;			/* Time of last access.  */
    // __syscall_ulong_t st_atimensec;	/* Nscecs of last access.  */
    // __time_t st_mtime;			/* Time of last modification.  */
    // __syscall_ulong_t st_mtimensec;	/* Nsecs of last modification.  */
    // __time_t st_ctime;			/* Time of last status change.  */
    // __syscall_ulong_t st_ctimensec;	/* Nsecs of last status change.  */
