//prog.c
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char path[256];
    if (getcwd(path, sizeof(path)) != NULL) {
        char *last_slash = strrchr(path, '/');
        if (last_slash != NULL) {
            printf("%s\n", last_slash + 1);
        } else {
            printf("hmmm.\n");
        }
    } else {
        perror("getcwd");
        return 1;
    }
    return 0;
}
