double randGen(int j, int shm){
	double temps;
	int shmid = shm;
	struct Voiture *getVoitures;
	int nombreSecteur = 3;
	srand(time(NULL)*getVoitures[j].id);

	for (int i = 0; i < nombreSecteur; i++ ){
		temps = rand() % 1600;
		temps = ( temps / 100 ) + 35;
		best(j, shmid, i, temps);
		// getVoitures[j].temps[i] = temps;
		getVoitures[j].temps[nombreSecteur] += temps;	
	best(j, shmid, 3, getVoitures[j].temps[nombreSecteur]);
	return 0;
}

void tour(int nbreTours, int shmid, int j){
	struct Voiture *getVoitures;
	if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
   		printf("Erreur : shmat\n");	
	}
	for(int i = 0; i < nbreTours; i++){
		randGen(j, shmid);
		//best()
	}

}

void best(int j, int shmid, int s, double nvTemps) {
	struct Voiture *setVoitures;
	if ((setVoitures = shmat(shmid, 0, 0)) == NULL) {
   		printf("Erreur : shmat\n");	
	}
	printf("Le nouveau %.2F et l'ancien %.2F\n", nvTemps,setVoitures[j].temps[s]);
	if (setVoitures[j].temps[s] > nvTemps) {
		setVoitures[j].temps[s] = nvTemps;
	}
	printf("Le nouveau %.2F\n", setVoitures[j].temps[s]);
}
