#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

/*Definition des fonctions du programme*/
int generateMatAdj_SHM(key_t key);

void bindMatAdj_SHM(int shm);

void setMatAdj_SHM(int value, int row, int column, int rowMax);

int getMatAdj_SHM(int row, int column, int rowMax);

void destroyMatAdj_SHM(int shm);

int generateSem(key_t key);

int P(int semId);

int V(int semId);

#endif /* FUNCTION_H_INCLUDED */
