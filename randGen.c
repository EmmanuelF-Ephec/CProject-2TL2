#include <stdio.h>
double randGen( double tempsTour[] ){
	double s;
	int i, n = 3;
	for ( i = 0; i < n; i++ ){
		s = rand() % 1600;
		s = ( s / 100 ) + 35;
		tempsTour[i] = s;
		tempsTour[3] += s;
	}
	return 0;
}
