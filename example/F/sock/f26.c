// f26.c
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int s;
struct sockaddr_un sa, sb;

void f(int sig){
    close(s);
    unlink(sa.sun_path);
    exit(0);
}

int main(int argc, char * argv[], char * envp[]){
    int l;
    char buf[80];
    
    if (argc != 3){
        fprintf(stderr, "Usage: %s localsocket remotesocket\n", argv[0]);
        exit(1);
    }
    
    if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    
    sa.sun_family = AF_UNIX;
    memset(sa.sun_path, 0, 108);
    strncpy(sa.sun_path, argv[1], 107);
    
    sb.sun_family = AF_UNIX;
    memset(sb.sun_path, 0, 108);
    strncpy(sb.sun_path, argv[2], 107);
    
    if (bind(s, (const struct sockaddr *)&sa, sizeof(sa)) == -1){
        perror("bind");
        exit(1);
    }
    
    signal(SIGALRM, f);
    
    while ((l = read(0, buf, 80)) > 0){
        sendto(s, buf, l, 0, (const struct sockaddr *)&sb, sizeof(sb));
        alarm(15);
        l = recv(s, buf, 80, 0);
        alarm(0);
        if (l > 0) write(1, buf, l);
    }
    
    kill(getpid(), SIGALRM);
}