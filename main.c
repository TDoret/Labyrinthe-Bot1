#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include "function.h"
#include "pile.h"

int main() {
    int a,b;
    int shm;            // identifiant de la memoire partagee
    int sem;               // identifiant du semaphore
    int ligMax = 25;
    int colMax = 25;
    key_t key;              // cle d'accès à la structure IPC
    Coord depart;
    Coord arrive;

    // generation de la cle
    key = 5678;

    printf("%s\n", "Début Programme");
    shm = generateMatAdj_SHM(key);
    sem = generateSem(key);

    //récupération et test en lecture de la SHM
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
    //Recup des coords d'arrive et dep
    P(sem);
    depart.x = getMatAdj_SHM(ligMax+1, colMax+1, ligMax);
    depart.y = getMatAdj_SHM(ligMax+2, colMax+2, ligMax);;
    arrive.x = getMatAdj_SHM(ligMax+3, colMax+3, ligMax);;
    arrive.y = getMatAdj_SHM(ligMax+4, colMax+4, ligMax);;
    V(sem);
    printf("Depart: %d:%d\n", depart.x, depart.y);
    printf("Arrivee: %d:%d\n", arrive.x, arrive.y);

    ResolveMaze(depart,arrive, ligMax, colMax, sem);
	//printf("Press Any Key to close bot\n");
	//getchar();
    printf("\n%s\n", "Fin Programme");

    return EXIT_SUCCESS;
}
