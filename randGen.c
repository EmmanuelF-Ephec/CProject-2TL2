
double randGen(int j, int shm){
	double s;
	int i;
	int shmid = shm;
	//int length = sizeof(tempsTour) / sizeof(tempsTour[0]);

	struct Voiture *getVoitures;
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
        printf("Erreur : shmat\n");
    }
	getVoitures[j].temps[3] = 0;

	for (i = 0; i < 3; i++ ){
		s = rand() % 1600;
		s = ( s / 100 ) + 35;
		getVoitures[j].temps[i] = s;
        printf( "S%d : %.2f secondes\n", (i+1), getVoitures[j].temps[i] );
		getVoitures[j].temps[3] += s;
	}
	printf( "Temps du tour : %.2f secondes\n", getVoitures[j].temps[3] );
	return 0;
}
