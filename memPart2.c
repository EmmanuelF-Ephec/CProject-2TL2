#include "memPart.c"
#include "library.h"

int shmid;
struct Voiture *getVoitures;

int getId;
double temps;


int main (int argc, char *argv[]) {
    if ((shmid = shmget(77, 20*sizeof(voitures), 0777)) == -1) {
        printf("Erreur : shmget");
        return -1;
    };
    if ((getVoitures = shmat(77, 0, 0777) == -1)) {
        printf("Erreur : shmat");
        return -1;
    }
    printf("L'id de la voiture est %d", getId);

    return 0;
}

