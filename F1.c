#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "randGen.c"
#include "comparTemps.c"
#include "fork.c"

struct Voiture {
    int num;
    double s1, s2, s3, tempsTotal;
};


int main (int argc, char *argv[]) {
    srand(time(NULL));
    double tempsTour[4];
    int voitures[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};



    int i, j;
    int length = sizeof(voitures) / sizeof(voitures[0]);
    forkVoitures(voitures);
    for ( j = 0; j < length; j++){
    randGen(tempsTour);
    printf( "Voiture %d\n", voitures[j]);
    for ( i = 0; i < 3; i++ ){
        printf( "S%d : %.2f secondes\n", (i+1), tempsTour[i] );
    }
    printf( "Temps du tour : %.2f secondes\n", tempsTour[3] );
    }
    return 0;
}
