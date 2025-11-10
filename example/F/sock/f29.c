// f29.c
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
    int s, l;
    char buf[80];
    struct sockaddr_in sa;
    struct hostent * h;
    
    if (argc != 2){
        fprintf(stderr, "Usage: %s host\n", argv[0]);
        exit(1);
    }
    
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
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
    memcpy(&sa.sin_addr, h->h_addr, h->h_length);
    
    if (connect(s, (const struct sockaddr *)&sa, sizeof(sa)) == -1) {
        perror("connect");
        exit(1);
    }
    
    if ((l = read(0, buf, 80)) == -1){
        perror("read");
        exit(1);
    }
    
    if (send(s, buf, l, 0) == -1){
        perror("send");
        exit(1);
    }
    
    alarm(10);
    if ((l = recv(s, buf, 80, 0)) == -1){
        perror("recv");
        exit(1);
    }
    alarm(0);
    
    if (write(1, buf, l) == -1){
        perror("write");
        exit(1);
    }
    
    close(s);
    exit(0);
}