// random.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define RANDOM_ARRAY_SIZE 4096 - (int)(sizeof(int))

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

static char * random_array = NULL;
static int * random_counter = NULL;
static int random_fd = -1;
static int random_shmid, random_semid;
//static int main_pid = -1;

void random_Exit() {
    // if (getpid() == main_pid) {
        if (shmdt(random_array) == -1) {
            perror("shmdt");
        }
        
        if (shmctl(random_shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
        }

        if (semctl(random_semid, 0, IPC_RMID) == -1) {
            perror("semctl");
        }

        if (close(random_fd) == -1) {
            perror("close");
        }

        random_array = NULL;
        random_counter = NULL;
        random_fd = -1;
    // }

    return;
}


static void random_SemBusy() {
    struct sembuf sb = {0, -1, 0};
    semop(random_semid, &sb, 1);
    return;
}

static void random_SemFree() {
    struct sembuf sb = {0, 1, 0};
    semop(random_semid, &sb, 1);
    return;
}

void random_Init() {
    int len;

    if (!random_array) {

        random_shmid = shmget(IPC_PRIVATE, RANDOM_ARRAY_SIZE + sizeof(int), IPC_CREAT | 0600);
        if (random_shmid == -1) {
            perror("shmget");
            exit(1);
        }

        random_array = (void *)shmat(random_shmid, NULL, 0);
        if (random_array == (void *)-1) {
            perror("shmat");
            exit(1);
        }

        random_counter = (int *)(random_array + RANDOM_ARRAY_SIZE);

        random_semid = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT | IPC_EXCL);
        if (random_semid == -1) {
            perror("semget");
            exit(1);
        }

        union semun arg;
        arg.val = 1;
        if (semctl(random_semid, 0, SETVAL, arg) == -1) {
            perror("semctl");
            exit(1);
        }

        // main_pid = getpid();
        // atexit(random_Exit);
    }

    if (random_fd == -1) {
        random_fd = open("/dev/urandom", O_RDONLY);
        // random_fd = open("/dev/random", O_RDONLY);
        if (random_fd == -1) {
            perror("open");
            exit(1);
        }
    }

    random_SemBusy();

    len = read(random_fd, random_array, RANDOM_ARRAY_SIZE);
    if(len == -1) {
        perror("read");
        exit(0);
    }

    * random_counter = 0;
    
    random_SemFree();

    return;
}

char random_GetChar() {
    int len;
    char res;

    random_SemBusy();

    if (* random_counter + sizeof(char) > RANDOM_ARRAY_SIZE) {
        * random_counter = 0;
        len = read(random_fd, random_array, RANDOM_ARRAY_SIZE);
        if(len == -1) {
            perror("read");
            exit(0);
        }
    }

    res = ((char *)random_array)[* random_counter];
    * random_counter += sizeof(char);

    random_SemFree();

    return res;
}

short random_GetShort() {
    int len;
    short res;

    random_SemBusy();

    if (* random_counter + sizeof(short) > RANDOM_ARRAY_SIZE) {
        * random_counter = 0;
        len = read(random_fd, random_array, RANDOM_ARRAY_SIZE);
        if(len == -1) {
            perror("read");
            exit(0);
        }
    }

    res = (short)(random_array[* random_counter]);
    * random_counter += sizeof(short);

    random_SemFree();

    return res;
}

int random_GetInt() {
    int len;
    int res;

    random_SemBusy();

    if (* random_counter + sizeof(int) > RANDOM_ARRAY_SIZE) {
        * random_counter = 0;
        len = read(random_fd, random_array, RANDOM_ARRAY_SIZE);
        if(len == -1) {
            perror("read");
            exit(0);
        }
    }

    res = (int)(random_array[* random_counter]);
    * random_counter += sizeof(int);

    random_SemFree();

    return res;
}

long random_GetLong() {
    int len;
    long res;

    random_SemBusy();

    if (* random_counter + sizeof(long) > RANDOM_ARRAY_SIZE) {
        * random_counter = 0;
        len = read(random_fd, random_array, RANDOM_ARRAY_SIZE);
        if(len == -1) {
            perror("read");
            exit(0);
        }
    }

    res = (long)(random_array[* random_counter]);
    * random_counter += sizeof(long);

    random_SemFree();

    return res;
}

long long random_GetLongLong() {
    int len;
    long long res;

    random_SemBusy();

    if (* random_counter + sizeof(long long) > RANDOM_ARRAY_SIZE) {
        * random_counter = 0;
        len = read(random_fd, random_array, RANDOM_ARRAY_SIZE);
        if(len == -1) {
            perror("read");
            exit(0);
        }
    }

    res = (long long)(random_array[* random_counter]);
    random_counter += sizeof(long long);

    random_SemFree();

    return res;
}

#define random_GetUnsignedChar()     (unsigned char)random_GetChar()
#define random_GetUnsignedShort()    (unsigned Short)random_GetShort()
#define random_GetUnsignedInt()      (unsigned Int)random_GetInt()
#define random_GetUnsignedLong()     (unsigned long)random_GetLong()
#define random_GetUnsignedLongLong() (unsigned long long)random_GetLongLong()

#ifdef RANDOM_MAIN
int main() {
    random_Init();

    printf("%d %d\n", random_GetInt(), * random_counter);
    printf("%lu %d\n", random_GetUnsignedLong(), * random_counter);
    printf("%lld %d\n", random_GetLongLong(), * random_counter);
    printf("%d\n", * random_counter);

    int p = fork();
    for (int i = 0; i < 64; i++) {
        printf("%d %d\n", random_GetShort(), * random_counter);
    }
    printf("\n");   

    if (p)
        wait(NULL);
    exit(0);
}
#endif /* RANDOM_MAIN */