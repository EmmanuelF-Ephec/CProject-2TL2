#include "library.h"

int main (int argc, char *argv[]) {
    int voitures[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};

    int shmid;
    struct Voiture *refVoitures;

    if ((shmid = shmget(77, 20*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };

    forkVoitures(voitures, shmid);
    return 0;
}
