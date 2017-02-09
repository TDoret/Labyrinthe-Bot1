#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "function.h"

#define LG_CHAINE 1024

int *adjM = NULL;    // pointeur d'attachement shared memory == matrice d'adjacence

int generateMatAdj_SHM(key_t key) {
    int shm;        // identifiant de la memoire partagee

    // creation du segment de memoire partagee
    if ((shm = shmget(key, LG_CHAINE, IPC_CREAT | 0600)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    printf("Genere SHM id: %d\n", shm);

    // attachement du segment shm sur le pointeur *chaine
    if ((adjM = shmat(shm, NULL, SHM_RDONLY)) == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shm;
}

int getMatAdj_SHM(int row, int column, int rowMax) {
    //printf("valeur retournee: %d\n", adjM[row * rowMax + column]);
    return adjM[row * rowMax + column];
}

void destroyMatAdj_SHM(int shm) {
    printf("Destruction SHM id: %d\n", shm);
    shmctl(shm, IPC_RMID, NULL);
}

int generateSem(key_t key)
{
    int sem;               // identifiant du semaphore

    // acces a l'ensemble semaphore et a la memoire partagee
    if ((sem = semget(key, 1, 0)) == -1) {
        perror("semget/shmget");
        exit(EXIT_FAILURE);
    }

    printf("Genere Sem id: %d\n", sem);
    return sem;
}


int P(int semId)
{
    struct sembuf buffer;
    buffer . sem_num = 0;
    buffer . sem_op = -1;
    buffer . sem_flg = 0;
    return (semop (semId, & buffer, 1));
}

int V(int semId)
{
    struct sembuf buffer;
    buffer . sem_num = 0;
    buffer . sem_op = 1;
    buffer . sem_flg = 0;
    return (semop (semId, & buffer, 1));
}







