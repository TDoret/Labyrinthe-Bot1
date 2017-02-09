#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include "pile.h"

/*Definition des fonctions du programme*/
int generateMatAdj_SHM(key_t key);

void bindMatAdj_SHM(int shm);

void setMatAdj_SHM(int value, int row, int column, int rowMax);

int getMatAdj_SHM(int row, int column, int rowMax);

void destroyMatAdj_SHM(int shm);

int generateSem(key_t key);

int P(int semId);

int V(int semId);

int isEqualCoord(Coord coordA, Coord coordB);

int ResolveMaze(Coord dep, Coord arr, int ligMax, int colMax, int sem);

#endif /* FUNCTION_H_INCLUDED */
