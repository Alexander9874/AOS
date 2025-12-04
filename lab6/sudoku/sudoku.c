// sudoku.c
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <wait.h>

#include "random.h"

static int semid;

struct sudoku_ds {
    char * rows[9];
    char grid_task[81];
    char grid_answer[81];
};

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

static void sudoku_SemBusy(unsigned sem) {
    struct sembuf oper[1] = {{sem, -1, 0}};

    if (semop(semid, oper, 1) == -1) {
        perror("semop");
        exit(1);
    }

    return;
}

static void sudoku_SemFree(unsigned sem) {
    struct sembuf oper[1] = {{sem, 1, 0}};

    if (semop(semid, oper, 1) == -1) {
        perror("semop");
        exit(1);
    }

    return;
}

static void sudoku_Sem2Busy(unsigned sem1, unsigned sem2) {
    struct sembuf opers[2] = {{sem1, -1, 0}, {sem2, -1, 0}};

    if (semop(semid, opers, 2) == -1) {
        perror("semop");
        exit(1);
    }

    return;
}

static void sudoku_Sem2Free(unsigned sem1, unsigned sem2) {
    struct sembuf opers[2] = {{sem1, 1, 0}, {sem2, 1, 0}};

    if (semop(semid, opers, 2) == -1) {
        perror("semop");
        exit(1);
    }

    return;
}

#if 0
static void sudoku_PrintTask(struct sudoku_ds * data) {
    for(int i = 0; i < 9; i++) {
        printf("%.9s\n", data->rows[i]);
    }
    printf("\n");
}

static void sudoku_PrintAnswer(struct sudoku_ds * data) {
    for(int i = 0; i < 81; i += 9) {
        printf("%.9s\n", data->grid_answer + i);
    }
    printf("\n");
}
#endif /* 0 */

static void sudoku_Init(struct sudoku_ds * data) {
    char * init = "123456789"
                  "456789123"
                  "789123456"
                  "234567891"
                  "567891234"
                  "891234567"
                  "345678912"
                  "678912345"
                  "912345678";

    memcpy(data->grid_task, init, 81);

    for (int i = 0; i < 9; i++) {
        data->rows[i] = data->grid_task + i * 9;
    }

    return;
}

static void sudoku_SwapRows(struct sudoku_ds * data, unsigned row1, unsigned row2) {
#ifndef SUDOKU_TRUST_ARGS
    if (row1 / 3 != row2 / 3 || row1 == row2) {
        fprintf(stderr, "ERROR illigal operation\n");
        exit(1);
    }
        
    if (row1 > 8 || row2 > 8) {
        fprintf(stderr, "ERROR rows are out of range\n");
        exit(1);
    }
#endif /* SUDOKU_TRUST_ARGS */

    sudoku_Sem2Busy(row1, row2);

    void * row_buf;

    row_buf = data->rows[row1];
    data->rows[row1] = data->rows[row2];
    data->rows[row2] = row_buf;

    sudoku_Sem2Free(row1, row2);

    return;
}

static void sudoku_SwapAreas(struct sudoku_ds * data, unsigned area1, unsigned area2) {
    void * rows_buf[3];

#ifndef SUDOKU_TRUST_ARGS
    if (area1 > 2 || area2 > 2) {
        fprintf(stderr, "ERROR areas are out of range\n");
        exit(1);        
    }

    if (area1 == area2) {
        fprintf(stderr, "ERROR illigal operation\n");
        exit(1);
    }
#endif /* SUDOKU_TRUST_ARGS */

    sudoku_Sem2Busy(area1, area2);

    memcpy(rows_buf, data->rows + area1 * 3, sizeof(void *) * 3);
    memcpy(data->rows + area1 * 3, data->rows + area2 * 3, sizeof(void *) * 3);
    memcpy(data->rows + area2 * 3, rows_buf, sizeof(void *) * 3);

    sudoku_Sem2Free(area1, area2);

    return;
}

static void sudoku_Trans(struct sudoku_ds * data) {
    char grid_buf[81];

    for (int i = 0; i < 9; i++) {
        memcpy(grid_buf + i * 9, data->rows[i], 9);
    }

    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            data->rows[r][c] = grid_buf[c * 9 + r];            
        }
    }

    return;
}

static void sudoku_Gen(struct sudoku_ds * data) {
    int swaps_left, pid, fork_count;
    fork_count = 0;

    for (swaps_left = (random_GetInt() & 7) | 16;
         swaps_left > 0;
         swaps_left--) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid) {
            fork_count++;
            if (fork_count > 7) {
                wait(NULL);
                fork_count--;
            }
            continue;
        }

        unsigned row1, row2, area;
        area = 3;
        area *= random_GetUnsignedInt() % 3;
        row1 = random_GetUnsignedInt() % 3;
        switch (row1) {
            case 0:
                row1 = 0;
                row2 = 1;
                break;
            case 1:
                row1 = 0;
                row2 = 2;
                break;
            case 2:
                row1 = 1;
                row2 = 2;
                break;
            default:
                fprintf(stderr, "How did we get here?\n");
                exit(1);
        }
        row1 += area;
        row2 += area;
        sudoku_SwapRows(data, row1, row2);

        exit(0);
    }

    for (; fork_count > 0; fork_count--)
        wait(NULL);

    for (swaps_left = (random_GetInt() & 3) | 4;
         swaps_left > 0;
         swaps_left--) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid) {
            fork_count++;
            if (fork_count > 1) {
                wait(NULL);
                fork_count--;
            }
            continue;
        }
        
        unsigned area1, area2;
        area1 = random_GetUnsignedInt() % 3;
        switch (area1) {
            case 0:
                area1 = 0;
                area2 = 1;
                break;
            case 1:
                area1 = 0;
                area2 = 2;
                break;
            case 2:
                area1 = 1;
                area2 = 2;
                break;
            default:
                fprintf(stderr, "How did we get here?\n");
                exit(1);
        }
        sudoku_SwapAreas(data, area1, area2);

        exit(0);
    }

    for ( ; fork_count > 0; fork_count--)
        wait(NULL);

    sudoku_Trans(data);

    for (swaps_left = (random_GetInt() & 7) | 16;
         swaps_left > 0;
         swaps_left--) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid) {
            fork_count++;
            if (fork_count > 7) {
                wait(NULL);
                fork_count--;
            }
            continue;
        }

        unsigned row1, row2, area;
        area = 3;
        area *= random_GetUnsignedInt() % 3;
        row1 = random_GetUnsignedInt() % 3;
        switch (row1) {
            case 0:
                row1 = 0;
                row2 = 1;
                break;
            case 1:
                row1 = 0;
                row2 = 2;
                break;
            case 2:
                row1 = 1;
                row2 = 2;
                break;
            default:
                fprintf(stderr, "How did we get here?\n");
                exit(1);
        }
        row1 += area;
        row2 += area;
        sudoku_SwapRows(data, row1, row2);

        exit(0);
    }

    for (; fork_count > 0; fork_count--)
        wait(NULL);

    for (swaps_left = (random_GetInt() & 3) | 4;
         swaps_left > 0;
         swaps_left--) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid) {
            fork_count++;
            if (fork_count > 1) {
                wait(NULL);
                fork_count--;
            }
            continue;
        }

        unsigned area1, area2;
        area1 = random_GetUnsignedInt() % 3;
        switch (area1) {
            case 0:
                area1 = 0;
                area2 = 1;
                break;
            case 1:
                area1 = 0;
                area2 = 2;
                break;
            case 2:
                area1 = 1;
                area2 = 2;
                break;
            default:
                fprintf(stderr, "How did we get here?\n");
                exit(1);
        }
        sudoku_SwapAreas(data, area1, area2);

        exit(0);
    }

    for (; fork_count > 0; fork_count--)
        wait(NULL);

    if (random_GetInt() & 1) {
        sudoku_Trans(data);
    }
    return;
}

static void sudoku_MakeAnswer(struct sudoku_ds * data) {
    for (int i = 0; i < 9; i++) {
        memcpy(data->grid_answer + i * 9, data->rows[i], 9);
    }
    return;
}

static void sudoku_CellRemove(struct sudoku_ds * data) {
    unsigned row_index, col_index;

    for (;;) {
        row_index = random_GetUnsignedInt() % 9;
        col_index = random_GetUnsignedInt() % 9;

        sudoku_SemBusy(row_index);

        if (data->rows[row_index][col_index] == ' ') {
            sudoku_SemFree(row_index);
            continue;
        }
        data->rows[row_index][col_index] = ' ';
        sudoku_SemFree(row_index);
        break;
    }

    return;
};

static void sudoku_Remove(struct sudoku_ds * data) {
    unsigned cells_remove;
    int pid, fork_count;
    fork_count = 0;
    
    for (cells_remove = (random_GetUnsignedInt() & 31) | 32;
         cells_remove > 0;
         cells_remove--) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid) {
            fork_count++;
            if (fork_count > 7) {
                wait(NULL);
                fork_count--;
            }
            continue;
        }
            
        sudoku_CellRemove(data);

        exit(0);
    }

    for (; fork_count > 0; fork_count--) {
        wait(NULL);
    }

    return;
}

static void sudoku_Export(struct sudoku_ds * sudoku, char * result) {
    for (int i = 0; i < 9; i++) {
        memcpy(result + i * 9, sudoku->rows[i], 9);
    }
    memcpy(result + 81, sudoku->grid_answer, 81);
    return;
}

void sudoku_ExportPrint(char * sudoku) {
    int i;
    for (i = 0; i < 81; i += 9) {
        printf("\t%.9s\n", sudoku + i);
    }
    printf("\n");
    for (; i < 162; i += 9) {
        printf("\t%.9s\n", sudoku + i);
    }
    return;
}

void sudoku_Main(char * result) {
    struct sudoku_ds * sudoku;

    random_Init();

    int shmid = shmget(IPC_PRIVATE, sizeof(struct sudoku_ds), 0600 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    sudoku = (void *)shmat(shmid,NULL, 0);
    if (sudoku == (void *)-1) {
            perror("shmat");
            exit(1);
    }

    semid = semget(IPC_PRIVATE, 9, 0600 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    unsigned short vals[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    union semun arg;
    arg.array = vals;
    if (semctl(semid, 0, SETALL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    sudoku_Init(sudoku);

    sudoku_Gen(sudoku);
    sudoku_MakeAnswer(sudoku);

    sudoku_Remove(sudoku);
    // sudoku_PrintTask(sudoku);
    // sudoku_PrintAnswer(sudoku);
    
    sudoku_Export(sudoku, result);
    
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
    }

    if (shmdt(sudoku)) {
        perror("shmdt");
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    random_Exit();

    return;
}

#ifdef GEN_SUDOKU_MAIN
int main() {
    char buf[162];
    sudoku_Main(buf);
    sudoku_ExportPrint(buf);
    exit(0);
}
#endif /* GEN_SUDOKU_MAIN */