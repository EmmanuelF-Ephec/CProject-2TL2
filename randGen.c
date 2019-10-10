#include <stdio.h>
double randGen( double tempsTour[4] ){
	double s;
	int i;
	for ( i = 0; i < 3; i++ ){
		s = rand() % 1600;
		s = ( s / 100 ) + 35;
		tempsTour[i] = s;
		tempsTour[3] += s;
	}
	return 0;
}
