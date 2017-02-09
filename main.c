#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include "function.h"

int main() {
    int a,b;
    int shm;            // identifiant de la memoire partagee
    int sem;               // identifiant du semaphore
    int ligMax = 10;
    int colMax = 10;
    key_t key;              // cle d'accès à la structure IPC

    // generation de la cle
    key = 5678;

    printf("%s\n", "Début Programme");
    shm = generateMatAdj_SHM(key);
    sem = generateSem(key);


    printf("%s\n", "----Lecture dans SHM----");
    printf("%s\n", "--MatAdj--");
    for (a = 0; a < ligMax; a++) {
        for (b = 0; b < colMax; b++) {
            P(sem);
            printf("%d |", getMatAdj_SHM(a, b, ligMax));
            V(sem);
        }
        printf("\n");
    }

	printf("Press Any Key to close bot\n");
	getchar();
    printf("\n%s\n", "Fin Programme");

    return EXIT_SUCCESS;
}
