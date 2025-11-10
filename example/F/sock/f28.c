// f28.c
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <dirent.h>

int main(int argc, char * argv[], char * envp[]) {
    int i, n, s, l;
    char buf[4097];
    struct sockaddr_in sa;
    struct hostent * h;
    
    if (argc != 2){
        fprintf(stderr, "Usage: %s host\n", argv[0]);
        exit(1);
    }
    
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    
    if ((h = gethostbyname(argv[1])) == NULL){
        herror(argv[1]);
        exit(1);
    }
    
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5000);
    memcpy(&sa.sin_addr, h->h_addr, 4);
    
    if (connect(s, (const struct sockaddr *)&sa, sizeof(sa)) == -1){
        perror("connect");
        exit(1);
    }
    
    while ((l = read(s, buf, 4096)) > 0){
        write(1, buf, l);
        buf[l] = '\0';
        if (strstr(buf, "\n\n") || l == 1)
            break;
    }
    
    printf("Enter file number: ");
    scanf("%d", &i);
    l = sprintf(buf, "%d\n", i);
    write(s, buf, l);
    
    if (read(s, buf, 1) != 1 || buf[0] != '\n'){
        write(2, "Error: ", 7);
        l = read(s, buf, 4096);
        if (l > 0) write(2, buf, l);
        write(2, "\n", 1);
        shutdown(s, SHUT_RDWR);
        close(s);
        exit(1);
    }
    
    while ((l = read(s, buf, 4096)) > 0)
        write(1, buf, l);
    
    shutdown(s, SHUT_RDWR);
    close(s);
    exit(0);
}