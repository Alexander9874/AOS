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
#include <arpa/inet.h>
#include <dirent.h>

int filter(const struct dirent * a){
    return a->d_type == 6;
}

int main(int argc, char * argv[], char * envp[]){
    int i, n, s, news, f, l;
    char buf[4096];
    struct dirent **namelist;
    struct sockaddr_in sa;
    
    if (argc > 2){
        fprintf(stderr, "Usage: %s [ directory ]\n", argv[0]);
        exit(1);
    }
    
    if (argc == 2) {
        if (chdir(argv[1])){
            perror("chdir");
            exit(1);
        }
    }
    
    if ((n = scandir(".", &namelist, filter, alphasort)) == -1){
        perror("scandir");
        exit(1);
    }
    
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){ 
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
    
    if (listen(s, 5) == -1){
        perror("listen");
        exit(1);
    }
    
    signal(SIGCHLD, SIG_IGN);
    
    for (;;){
        if ((news = accept(s, NULL, NULL)) == -1){ 
            perror("accept");
            exit(1);
        }
        
        if (fork()) {
            close(news);
        } else {
            close(s);
            close(0);
            close(1);
            close(2);
            dup(news);
            dup(news);
            dup(news);
            setbuf(stdin, NULL);
            setbuf(stdout, NULL);
            setbuf(stderr, NULL);
            
            for (i = 0; i < n; i++)
                printf("%d %s\n", i+1, namelist[i]->d_name);
            printf("\n");
            
            scanf("%d", &i);
            if (i > 0 && i <= n){
                if ((f = open(namelist[i-1]->d_name, O_RDONLY)) == -1)
                    perror(namelist[i-1]->d_name);
                else {
                    printf("\n");
                    while((l = read(f, buf, 4096)) > 0)
                        write(1, buf, l);
                    if (l == 0)
                        printf("\n\n");
                    else
                        perror("read");
                    close(f);
                }
            }
            
            for (i = 0; i < n; i++)
                free(namelist[i]);
            free(namelist);
            
            shutdown(news, SHUT_RDWR);
            close(news);
            exit(0);
        }
    }
}