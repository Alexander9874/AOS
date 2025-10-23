// menu.c
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>

#include "wrapper.h"

#define FD_STD_IN  0
#define FD_STD_OUT 1
#define FD_STD_ERR 2

long menu_ReadInt(const char * const msg)
{
    int ret_code = 0;
    long num;
    while (!ret_code) {
        _printf("%s", msg);
        ret_code = scanf("%ld", &num);
        if (ret_code == -1) {
            if (!errno)
                return 0;
            perror("scanf");
            exit(1);
        }
        if (ret_code == 0) {
            _printf("Wrong input! Try again.\n");
			scanf("%*s");
        }
    }
    return num;
}

void menu_Read(int fd) {
    _printf("Seek your data in -> <- below\n"
            "->");
    fflush(stdout);
    rw(fd, FD_STD_OUT);
    _printf("<-\n");
    return;
}

void menu_Write(int fd) {
    _printf("Write your data below\n");
    rw(FD_STD_IN, fd);
    return;
}

void menu_Lseek(int fd) {
    int answer, whence;
    off_t offset, cur_offset, flen, tmp = 0;
    const char * const msg_offset = "Seek offset is\n";
    const char * const msg_whence = "Seek whence is\n"
                                    "1 - SEEK_SET\n"
                                    "2 - SEEK_CUR\n"
                                    "3 - SEEK_END\n"
                                    # ifdef __USE_GNU
                                    "4 - SEEK_DATA\n"
                                    "5 - SEEK_HOLE\n"
                                    #endif // __USE_GNU
                                    "0 - EXIT\n";

    cur_offset = _lseek(fd, 0, SEEK_CUR);
    flen = _lseek(fd, 0, SEEK_END);
    _lseek(fd, cur_offset, SEEK_SET);

    while (1) {
        _printf("Current caret posotion is %ld\n"
                "Current file length is %ld\n",
                cur_offset, flen);

        while (1) {
            answer = menu_ReadInt(msg_whence);
            if (answer < 0 || answer > 5) {
                _printf("Wrong number! Try again!\n");
                continue;
            }
            break;
        }

        if (!answer)
            return;

        whence = answer - 1;
        offset = menu_ReadInt(msg_offset);

        tmp = whence == SEEK_SET ? offset
            : whence == SEEK_CUR ? cur_offset + offset
            : whence == SEEK_END ? flen + offset
            : 0; // __USE_GNU good luck

        if (tmp < 0) {
            _printf("Result position is out of range! Try again!\n");
            continue;
        }
        break;
    }

    answer = _lseek(fd, offset, whence);

    _printf("Caret moved to %d\n", answer);
    return;
}

// #include <dirent.h>
// #include <sys/stat.h>
// void menu_Unlink(int fd) {
//     // AT_EMPTY_DIR
//     if(unlinkat(fd, "", 0x1000)) {
//         perror("unlinkat");
//         exit(1);
//     }
//     return;
// }

void menu_Exit(int fd) {
    _close(fd);
    exit(0);
}

typedef void (*func_t)(int);

void menu_Main(int fd) {
    func_t fun_vect[] = {menu_Exit,
                         menu_Read,
                         menu_Write,
                         menu_Lseek};
    int answer;
    char * msg = "What to do?\n"
       "1 - READ\n"
       "2 - WRITE\n"
       "3 - LSEEK\n"
    //    "4 - UNLINK\n"
       "0 - EXIT\n";
    while (1) {
        answer = menu_ReadInt(msg);
        if (answer & -4) {
            _printf("Wrong number! Try again!\n");
            continue;
        }
        fun_vect[answer](fd);
    }
    return;
}
