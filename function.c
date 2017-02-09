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
#include "pile.h"


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

int isEqualCoord(Coord coordA, Coord coordB)
{
    if(coordA.y == coordB.y && coordA.x == coordB.x)
    {
        printf("Vous êtes sorti !\n");
        printf("Pos de sorti => %d:%d\n",coordB.x,coordB.y);
        return 1;
    }
    else
        return 0;
}

int ResolveMaze(Coord dep, Coord arr, int ligMax, int colMax, int sem)
{
    Coord botPos;
    int a,b;
    int isValidDirect = 0;
    int isNoDirect = 0;
    int adj;
    int tabVis[ligMax][colMax];
    Pile *maPile = initialiser();
    Coord coordTemp;

    for (a = 0; a < ligMax; a++) {
        for (b = 0; b < colMax; b++) {
            tabVis[a][b] = 0;
        }
    }

    //je positionne le bot sur le depart 
    botPos.x = dep.x;
    botPos.y = dep.y;
    coordTemp.x = botPos.x;
    coordTemp.y = botPos.y;
    empiler(maPile, coordTemp);
    printf("---- Debut Algo ----\n");
    while(isEqualCoord(botPos,arr) == 0)
    {
        isValidDirect = 0;
        isNoDirect = 0;

        do
        {
            printf("pos : %d:%d\n",botPos.x,botPos.y);
            tabVis[botPos.x][botPos.y] = 1;

            //je get l'adj
            P(sem);
            adj = getMatAdj_SHM(botPos.x, botPos.y, ligMax);
            V(sem);
            printf("adj : %d\n",adj);

            switch(adj)
            {
                //up
                case 1:
                    //haut
                    if(tabVis[botPos.x-1][botPos.y] == 0)
                    {
                        botPos.x--;
                        isValidDirect = 1;
                    }
                    else
                    {
                        isNoDirect = 1;
                    }
                    break;
                //right
                case 2:
                    //droite
                    if(tabVis[botPos.x][botPos.y + 1] == 0)
                    {
                        botPos.y ++;
                        isValidDirect = 1;
                    }
                    else
                    {
                        isNoDirect = 1;
                    }
                    break;
                //up:right
                case 3:
                    //haut
                    if(tabVis[botPos.x - 1][botPos.y] == 0)
                    {
                        botPos.x--;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //droite
                        if(tabVis[botPos.x][botPos.y+1] == 0)
                        {
                            botPos.y ++;
                            isValidDirect = 1;
                        } 
                        else
                        {
                            isNoDirect = 1;
                        }
                    }
                    break;
                //down
                case 4:
                    //bas
                    if(tabVis[botPos.x+1][botPos.y] == 0)
                    {
                        botPos.x ++;
                        isValidDirect = 1;
                    }
                    else
                    {
                        isNoDirect = 1;
                    }
                    break;
                //up:down
                case 5:
                    //haut
                    if(tabVis[botPos.x-1][botPos.y] == 0)
                    {
                        botPos.x--;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //bas
                        if(tabVis[botPos.x+1][botPos.y] == 0)
                        {
                            botPos.x ++;
                            isValidDirect = 1;
                        }
                        else
                        {
                            isNoDirect = 1;
                        }
                    }
                    break;
                //right:down
                case 6:
                    //droite
                    if(tabVis[botPos.x][botPos.y+1] == 0)
                    {
                        botPos.y ++;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //bas
                        if(tabVis[botPos.x+1][botPos.y] == 0)
                        {
                            botPos.x ++;
                            isValidDirect = 1;
                        } 
                        else
                        {
                            isNoDirect = 1;
                        }
                    }
                    break;
                //up:right:down
                case 7:
                    //haut
                    if(tabVis[botPos.x-1][botPos.y] == 0)
                    {
                        botPos.x--;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //droite
                        if(tabVis[botPos.x][botPos.y+1] == 0)
                        {
                            botPos.y ++;
                            isValidDirect = 1;
                        } 
                        else
                        {
                            //bas
                            if(tabVis[botPos.x+1][botPos.y] == 0)
                            {
                                botPos.x ++;
                                isValidDirect = 1;
                            } 
                            else
                            {
                                isNoDirect = 1;
                            }
                        }
                    }
                    break;
                //only left
                case 8:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y --;
                        isValidDirect = 1;
                    }
                    else
                    {
                        isNoDirect = 1;
                    }
                    break;
                //left:up
                case 9:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y --;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //haut
                        if(tabVis[botPos.x-1][botPos.y] == 0)
                        {
                            botPos.x--;
                            isValidDirect = 1;
                        }
                        else
                        {
                            isNoDirect = 1;
                        }
                    }
                    break;
                //left:right
                case 10:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y --;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //droite
                        if(tabVis[botPos.x][botPos.y+1] == 0)
                        {
                            botPos.y ++;
                            isValidDirect = 1;
                        } 
                        else
                        {
                            isNoDirect = 1;
                        }
                    }                
                    break;
                //left:up:right
                case 11:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y --;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //haut
                        if(tabVis[botPos.x-1][botPos.y] == 0)
                        {
                            botPos.x--;
                            isValidDirect = 1;
                        }
                        else
                        {
                            //droite
                            if(tabVis[botPos.x][botPos.y+1] == 0)
                            {
                                botPos.y ++;
                                isValidDirect = 1;
                            }
                            else
                            {
                                isNoDirect = 1;
                            }
                        }
                    }
                    break;
                //left:down
                case 12:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y --;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //bas
                        if(tabVis[botPos.x+1][botPos.y] == 0)
                        {
                            botPos.x ++;
                            isValidDirect = 1;
                        } 
                        else
                        {
                            isNoDirect = 1;
                        }
                    }
                    break;
                //left:up:down
                case 13:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y --;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //haut
                        if(tabVis[botPos.x-1][botPos.y] == 0)
                        {
                            botPos.x--;
                            isValidDirect = 1;
                        }
                        else
                        {
                            //bas
                            if(tabVis[botPos.x+1][botPos.y] == 0)
                            {
                                botPos.x ++;
                                isValidDirect = 1;
                            }
                            else
                            {
                                isNoDirect = 1;
                            }
                        }
                    }
                    break;
                //left:right:down
                case 14:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y --;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //droite
                        if(tabVis[botPos.x][botPos.y+1] == 0)
                        {
                            botPos.y ++;
                            isValidDirect = 1;
                        }
                        else
                        {
                            //bas
                            if(tabVis[botPos.x+1][botPos.y] == 0)
                            {
                                botPos.x ++;
                                isValidDirect = 1;
                            }
                            else
                            {
                                isNoDirect = 1;
                            }
                        }
                    }
                    break;
                //all dir
                case 15:
                    //Gauche
                    if(tabVis[botPos.x][botPos.y-1] == 0)
                    {
                        botPos.y--;
                        isValidDirect = 1;
                    }
                    else
                    {
                        //haut
                        if(tabVis[botPos.x-1][botPos.y] == 0)
                        {
                            botPos.x--;
                            isValidDirect = 1;
                        }
                        else
                        {
                            //droite
                            if(tabVis[botPos.x][botPos.y+1] == 0)
                            {
                                botPos.y ++;
                                isValidDirect = 1;
                            }
                            else
                            {
                                //bas
                                if(tabVis[botPos.x+1][botPos.y] == 0)
                                {
                                    botPos.x ++;
                                    isValidDirect = 1;
                                }
                                else
                                {
                                    isNoDirect = 1;
                                }
                            }
                        }
                    }
                    break;
            }

            if(isNoDirect == 1)
            {
                    printf("%s\n", "Aucune direction Possible !");
                    //getchar();
                    coordTemp = depiler(maPile);
                    botPos.x = coordTemp.x;
                    botPos.y = coordTemp.y;
                    isValidDirect = 0;
                    isNoDirect = 0;
            }
            else
            {
                printf("j'empile la pos %d:%d\n",botPos.x,botPos.y);
                //getchar();
                coordTemp.x = botPos.x;
                coordTemp.y = botPos.y;
                empiler(maPile, coordTemp);
            }

        }while(isValidDirect == 0);
    }
    return EXIT_SUCCESS;
}






