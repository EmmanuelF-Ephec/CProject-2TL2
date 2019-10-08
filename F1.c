#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

double randGen(){
	srand(time(NULL));
	double tempsTour[4], s;
	int i, n = 3;
	for ( i = 0; i < n; i++ ){
		s = (( rand() % 1600 ) / 100 ) + 35;
		tempsTour[i] = s;
		tempsTour[3] += s;
		printf("S%d : %.2f secondes\n", (i+1), tempsTour[i]);
	}
	printf("Temps du tour : %.2f secondes\n", tempsTour[3]);
	return 0;
}
int main (int argc, char *argv[]) {

    int pid_fils;
    int voitures[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};

    int pere = getpid();
    printf("pere %d\n" , pere);
    for (int i = 0; i < 4; i++)
    {
    
        pid_fils = fork();
        if (pid_fils == 0) {
            int num = voitures[i];
            printf("La voiture avec le numéro %d a fait la section 1 avec le temps %d \n",num, timer());
            //printf("Le fils avec pid %d et num %d et pere a pid %d\n",getpid(),num, getppid()); 
            exit(0);
        }
        else
        {
            printf("je suis le pere\n");
        }
        
    }

    return 0;
}