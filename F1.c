#include "library.h"

int main (int argc, char *argv[]) {
    srand(time(NULL));
    double tempsTour[4];
    int voitures[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};

    int shmid;
    struct Voiture *refVoitures;

    if ((shmid = shmget(77, 20*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };

    forkVoitures(voitures, shmid);

    int i, j;
    int length = sizeof(voitures) / sizeof(voitures[0]);

  /*  for ( j = 0; j < length; j++){
        tempsTour[3] = 0;
        randGen(tempsTour);
        printf( "Voiture %d\n", voitures[j]);
        for ( i = 0; i < 3; i++ ){
            printf( "S%d : %.2f secondes\n", (i+1), tempsTour[i] );
        }
        printf( "Temps du tour : %.2f secondes\n", tempsTour[3] );
        }
*/
    return 0;
}
