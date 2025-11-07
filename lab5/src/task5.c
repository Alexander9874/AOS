// task5.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "wrapper.h"

int main(int argc, char *argv[]) {
    int msgid; 
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s msgid\n", argv[0]);
        exit(1);
    }

    msgid = _strtol_errmsg(argv[1], 10, "wrong msgid");

    _msgctl(msgid, IPC_RMID, NULL);

    exit(0);
}
