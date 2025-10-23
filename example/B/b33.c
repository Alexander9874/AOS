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

#define NAME "/tmp/configfile.configfile"
#define MAXNAME 255

extern int errno;
char name[MAXNAME+1], *message;

void mydaemon() {
    setsid();
    openlog("mydaemon", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "%s", "Started");
    syslog(LOG_INFO, "%s", message);
    for (;;) pause();
}

void term() {
    syslog(LOG_INFO, "%s", "Finished");
    free(message);
    closelog();
    exit(0);
}

void hup() {
    int f;
    struct stat st;
    free(message);
    message = NULL;
    if ((f = open(name, O_RDONLY)) == -1) {
        syslog(LOG_INFO, "%s: %s", name, strerror(errno));
        term();
    }
    if (fstat(f, &st) == -1) {
        syslog(LOG_INFO, "%s: %s", name, strerror(errno));
        term();
    }
    if ((message = malloc(st.st_size + 1)) == NULL) {
        syslog(LOG_INFO, "%s: %s", "malloc", strerror(errno));
        term();
    }
    read(f, message, st.st_size);
    message[st.st_size] = '\0';
    close(f);
    syslog(LOG_INFO, "%s", "Reconfigured");
    syslog(LOG_INFO, "%s", message);
}

void usr(int sig) {
    char *p;
    if (sig == SIGUSR1) p = "Increment debug level";
    else p = "Decrement debug level";
    syslog(LOG_INFO, "%s", p);
}

int main(int argc, char *argv[], char *envp[]) {
    char *p;
    struct stat st;
    struct sigaction act;
    int f;
    
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [ configfile ]\n", argv[0]);
        exit(1);
    }
    
    if (argc > 1) {
        if (strlen(argv[1]) > MAXNAME) {
            fprintf(stderr, "Name '%s' of configfile too long\n", argv[1]);
            exit(1);
        }
        strcpy(name, argv[1]);
    }
    else if ((p = getenv("CONFIG")) != NULL) {
        if (strlen(p) > MAXNAME) {
            fprintf(stderr, "Name '%s' of configfile too long\n", p);
            exit(1);
        }
        strcpy(name, p);
    }
    else {
        strcpy(name, NAME);
    }
    
    if ((f = open(name, O_RDONLY)) == -1) {
        perror(name);
        exit(1);
    }
    
    if (fstat(f, &st) == -1) {
        perror(name);
        exit(1);
    }
    
    if ((message = malloc(st.st_size + 1)) == NULL) {
        perror("malloc");
        exit(1);
    }
    
    read(f, message, st.st_size);
    message[st.st_size] = '\0';
    close(f);
    
    act.sa_handler = hup;
    sigfillset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    sigaction(SIGHUP, &act, NULL);
    
    act.sa_handler = term;
    sigaction(SIGTERM, &act, NULL);
    
    act.sa_handler = usr;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    
    for (f = 0; f < 256; f++)
        close(f);
    chdir("/");
    
    f = fork();
    switch (f) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            mydaemon();
    }
    exit(0);
}