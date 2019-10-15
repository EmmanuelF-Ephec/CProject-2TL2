#include "library.h"


int shmid;
struct Voiture *voitures[20];

int main (int argc, char *argv[]) {

    printf(1);
    /*if ((shmid = shmget(77, 20*sizeof(voitures), 0777)) == -1) {
        printf(sizeof(voitures));
        printf("Erreur : shmget\n");
        return -1;
    };
    if ((voitures = shmat(77, 0, 0777) == -1)) {
        printf("Erreur : shmat\n");
        return -1;
    }
*/
    return 0;
}