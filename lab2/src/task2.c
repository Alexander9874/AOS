#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "wrapper.h"

int main() {
    int ret_val;
    if ((ret_val = _fork())) {
        _printf("I'm father.\n");
    }
    else {
        _printf("I'm son.\n");
    }
    exit(0);
}
