#include "library.h"

int pid_fils;
int shmid;
struct Voiture *voitures;
struct Voiture *getVoitures;

int main (int argc, char *argv[]) {

    if ((shmid = shmget(77, 20*sizeof(voitures), 0777)) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };
    if ((voitures = shmat(shmid, 0, 0)) == NULL) {
        printf("Erreur : shmat\n");
        return -1;
    }    
    else {
        voitures[0].id = 1;
    }

    for (int i = 0; i < 3; i++) {
    
        pid_fils = fork();
        if (pid_fils == 0) {
            if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
                printf("Erreur : shmat");
                return -1;
             }
            printf("%d\n",getVoitures[0].id);
        }
    }
    return 0;
}