// f30.c
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

int main(){
    int s, l, len, i;
    char buf[80];
    struct sockaddr_in sa;
    
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5000);
    sa.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(s, (const struct sockaddr *)&sa, sizeof(sa)) == -1){
        perror("bind");
        exit(1);
    }
    
    for (;;){
        len = sizeof(sa);
        if ((l = recvfrom(s, buf, 80, 0, (struct sockaddr *)&sa, &len)) == -1){
            perror("recvfrom");
            exit(1);
        }
        
        if (!strncmp(buf, "exit", 4))
            break;
        
        for (i = 0; i < (l - 1) / 2; i++){
            buf[i] = buf[l - i - 2] ^ buf[1];
            buf[l - i - 2] = buf[1 - i - 2] ^ buf[i];
            buf[i]=buf[l - i - 2] ^ buf[i];
        }
        
        if (sendto(s, buf, l, 0, (const struct sockaddr *)&sa, len) == -1){
            perror("sendto");
            exit(1);
        }
    }
    
    buf[0] = '\0';
    if (sendto(s, buf, 1, 0, (const struct sockaddr *)&sa, len) == -1){
        perror("sendto");
        exit(1);
    }
    
    close(s);
    exit(0);
}