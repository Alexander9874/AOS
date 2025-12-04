#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sudoku.h"

#define PORT 42067
#define BUFFER_SIZE 162

int main(int argc, char ** argv) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    int len, val;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s IP\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    for (;;) {
        len = read(sock, buffer, BUFFER_SIZE);
        if (len == -1) {
            perror("read");
            exit(1);
        }

        if (len == BUFFER_SIZE) {
            sudoku_ExportPrint(buffer);
            continue;
        }

        buffer[len] = '\0';
        printf("%s\n", buffer);

        len = scanf("%d", & val);
        if (len == -1) {
            val = 0;
        }
        if (len == 0) {
            val = -1;
            scanf("%*s");
        }
        
        if (send(sock, & val, sizeof(int), 0) == -1) {
            perror("send");
            exit(1);
        }

        if (!val)
            break;
    }

    close(sock);
    exit(0);
}