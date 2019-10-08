#include <stdio.h>
#include <unistd.h>

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
