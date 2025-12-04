// sudoku_server.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <bits/types/sigset_t.h>
#include <bits/sigaction.h>
#include <time.h>

#include "sudoku.h" 

#ifndef SA_RESTART
#   define SA_RESTART   0x10000000
#   error "SA_RESTART"
#endif /* SA_RESTART */

#ifndef SO_REUSEPORT
#   define SO_REUSEPORT 15
#   error "SO_REUSEPORT"
#endif /* SO_REUSEPORT */

#ifndef SA_NODEFER
#   define SA_NODEFER   0x40000000
#   error "SA_NODEFER"
#endif /* SA_NODEFER */

int PORT = 0;
int MAX_CONNECTIONS = 0;
char * CONF_FILENAME =
       "/home/alexander/Documents/AOS/lab6/sudoku/configfile.conf";
char * ABS_PATH =
       "/home/alexander/Documents/AOS/lab6/sudoku/server.out";

int CHLD_NUM = 0;

void server_Terminate(int exit_code) {
    fprintf(stdout, "Finished: pid %d exit_code %d\n", getpid(), exit_code);
    exit(exit_code);
}

void server_WaitChld() {
    sigset_t mask, oldmask;
    sigset_t suspend_mask;
    
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigemptyset(&suspend_mask);
        
    for (; CHLD_NUM != 0;) {
        sigsuspend(&suspend_mask);
    }
    
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

void server_Serve(int socket_fd) {
    char * msg_send = "Sudoku:\n"
                      "1. Generate\n"
                      "0. Exit";
    char msg_sudoku[162];
    int msg_rec;
    int len;

    struct sigaction act;
    sigfillset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    act.sa_handler = SIG_DFL;
    if (sigaction(SIGCLD, &act, NULL) == -1 ) {
        perror("sigaction");
        server_Terminate(1);
    }

    for (;;) {
        if(send(socket_fd, msg_send, 29, 0) == -1) {
            perror("send");
            exit(1);
        }
        
        len = read(socket_fd, & msg_rec, 4);
        if(len == -1) {
            perror("read");
            exit(1);
        }

        if (len == 4) {
            if (!msg_rec)
                break;
            if (msg_rec == 1) {
                sudoku_Main(msg_sudoku);
                if(send(socket_fd, msg_sudoku, 162, 0) == -1) {
                    perror("send");
                    exit(1);
                }
            }
        }
    }

    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
    exit(0);
}

void server_Init() {
    int server_fd, socket_fd;
    struct sockaddr_in address;
    int addrlen;
    int ret_val, opt;
    pid_t fork_pid;

    addrlen = sizeof(address);
    opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        server_Terminate(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                &opt, sizeof(opt))) {
        perror("setsockopt");
        server_Terminate(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    ret_val = bind(server_fd, (struct sockaddr *)&address, sizeof(address)); 
    if (ret_val == -1) {
        perror("bind");
        server_Terminate(1);
    }

    ret_val = listen(server_fd, MAX_CONNECTIONS); 
    if (ret_val == -1) {
        perror("listen");
        server_Terminate(1);
    }

    printf("Started: pid %d PORT %d MAX_CONNECTIONS %d\n",
           getpid(), PORT, MAX_CONNECTIONS);

    for(;;) {
        socket_fd = accept(server_fd,
                           (struct sockaddr *)&address,
                           (socklen_t*)&addrlen);
        if (socket_fd == -1) {
            perror("accept");
            server_Terminate(1);
        }

        fork_pid = fork();
        if (fork_pid == -1) {
            perror("fork");
            server_Terminate(1);
        }

        CHLD_NUM++;

        if(!fork_pid) {
            printf("Established conection: pid %d connections %d\n", getpid(), CHLD_NUM);
            
            close(server_fd);
            server_Serve(socket_fd);
        }
        close(socket_fd);
    }
}

void Trim(char *str) {
    char * start = str;
    char * end = str + strlen(str) - 1;

    while (* start == ' ' || * start == '\t')
        start++;

    while ((* end == ' ' || * end == '\t') && end > start)
        end--;
    
    memmove(str, start, end - start + 1);

    str[end - start + 1] = '\0';
}

void Reconfig() {
    FILE * stream;
    char line[4096 + 256], logfile_name[4096] = {0};
    int line_counter, port = 0, max = 0;
    time_t t;
    struct tm *tm_info;
    char time_buffer[27];

    stream = fopen(CONF_FILENAME, "r");
    if (stream == NULL) {
        fprintf(stderr, "ERROR \"%s\":", CONF_FILENAME);
        perror("fopen");
        server_Terminate(1);
    }

    line_counter = 0;
    while(fgets(line, 4096 + 256, stream)) {
        char * key, * value, * delimiter;
        int value_len, port_tmp, max_tmp;

        line_counter++;
        line[strcspn(line, "\n")] = '\0';
        Trim(line);
        
        if (strlen(line) == 0 || line[0] == '#')
            continue;

        delimiter = strchr(line, '=');
        if (delimiter == NULL) {
            fprintf(stderr,
                    "WARNING in line %d: unrecignized statement\n",
                    line_counter);
            continue;
        }

        *delimiter = '\0';
        key = line;
        value = delimiter + 1;
        Trim(key);
        Trim(value);
        value_len = strlen(value);

        if (strcmp(key, "PORT") == 0) {
            if (sscanf(value, "%d", & port_tmp) != 1) {
                fprintf(stderr,
                        "WARNING in line %d: unrecignized value PORT\n",
                        line_counter);
                continue;
            }
            if (port) {
                fprintf(stderr,
                        "WARNING in line %d: value PORT is already "
                        "set as %d, %d is discarded\n",
                        line_counter, port, port_tmp);
                continue;
            }
            port = port_tmp;
            continue;
        }

        if (strcmp(key, "MAX_CONNECTIONS") == 0) {
            if (sscanf(value, "%d", & max_tmp) != 1) {
                fprintf(stderr,
                        "WARNING in line %d: unrecignized value MAX_CONNECTIONS\n",
                        line_counter);
                continue;
            }
            if (max) {
                fprintf(stderr,
                        "WARNING in line %d: value PORT is already "
                        "set as %d, %d is discarded\n",
                        line_counter, max, max_tmp);
                continue;
            }
            max = max_tmp;
            continue;
        }

        if (strcmp(key, "LOGFILE") == 0) {
            if (value[0] == '"' && value[value_len - 1] == '"') {
                if (* logfile_name) {
                    fprintf(stderr,
                            "WARNING in line %d: value LOGFLE is already "
                            "set as \"%s\", %s is discarded.\n",
                            line_counter, logfile_name, value);
                    continue;
                }
                strncpy(logfile_name, value + 1, value_len - 2);
                logfile_name[strlen(value) - 2] = '\0';
            }
            else {
                fprintf(stderr,
                        "WARNING in line %d: unrecognized value LOGFILE\n",
                        line_counter);
                continue;
            }
            continue;
        }
        fprintf(stderr,
                "WARNING in line %d: unrecognized key\n",
                line_counter);
    }
    
    
    time(&t);
    tm_info = localtime(&t);
    strftime(time_buffer, 27, ".%YY%mM%dD%Hh%Mm%Ss", tm_info);
    strcat(logfile_name, time_buffer);
   
    fclose(stream);

    stream = fopen(logfile_name, "a");
    if (stream == NULL) {
        fprintf(stderr, "ERROR \"%s\":", logfile_name);
        perror("fopen");
        server_Terminate(1);
    }
    setbuf(stream, NULL);

    fflush(NULL);
    if (stderr != stdout) {
        fclose(stdout);
        fclose(stdin);
    }
    fclose(stderr);

    stdout = stream;
    stderr = stream;

    dup2(stream->_fileno, 1);
    dup2(stream->_fileno, 2);

    PORT = port;
    MAX_CONNECTIONS = max;

    return;
}

void CloseAllFds() {
    long open_max = sysconf(_SC_OPEN_MAX);
    if (open_max == -1) {
        open_max = 1024;
    }
    
    for (int fd = 3; fd < open_max; fd++) {
        close(fd);
    }
}

void SigHupDisp(int sig) {
    server_WaitChld();
    printf("Signaled SIGHUP(%d): Reconfiguring\n", sig);
    execl(ABS_PATH, ABS_PATH, NULL);
    perror("execl");
    server_Terminate(1);
}

void SigTermDisp(int sig) {
    server_WaitChld();
    printf("Signaled SIGTERM(%d)\n", sig);
    server_Terminate(0);
    return;
}

void SigChldDisp(int sig) {
    int pid, exit_code;
    pid = wait(& exit_code);
    if (pid == -1) {
        perror("wait");
        server_Terminate(1);
    }
    CHLD_NUM--;
    printf("Signaled SIGCHLD(%d): pid %d finished %d connections %d\n", sig, pid, exit_code, CHLD_NUM);
    return;
}

int main(int argc, char ** argv) {
    struct sigaction act;
    pid_t fork_pid;

    if (argc > 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        exit(1);
    }

    fork_pid = fork();
    if (fork_pid == -1) {
        perror("fork");
        exit(1);
    }

    if (fork_pid == 0) {
        chdir("/");
        setsid();
        CloseAllFds();

        sigset_t set;
        sigemptyset(&set);
        sigprocmask(SIG_SETMASK, &set, NULL);
        
        sigfillset(&act.sa_mask);
        act.sa_flags = SA_RESTART | SA_NODEFER;

        act.sa_handler = SigHupDisp;
        if (sigaction(SIGHUP, &act, NULL) == -1 ) {
            perror("sigaction");
            server_Terminate(1);
        }
        
        act.sa_handler = SigTermDisp;
        if (sigaction(SIGTERM, &act, NULL) == -1 ) {
            perror("sigaction");
            server_Terminate(1);
        }

        // act.sa_flags |= SA_NODEFER;
        act.sa_handler = SigChldDisp;
        if (sigaction(SIGCHLD, &act, NULL) == -1 ) {
            perror("sigaction");
            server_Terminate(1);
        }

        Reconfig();
        server_Init();
    }

    sleep(1);
    exit(0);
}