double randGen(int j, int shm){
	double temps;
	int shmid = shm;
	struct Voiture *getVoitures;
	if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
   		printf("Erreur : shmat\n");	
	}
	size_t nombreSecteur = sizeof(getVoitures[j].temps)/sizeof(getVoitures[j].temps[0]) - 1;
	getVoitures[j].temps[nombreSecteur] = 0;
	srand(time(NULL)*getVoitures[j].id);

	for (int i = 0; i < nombreSecteur; i++ ){
		temps = rand() % 1600;
		temps = ( temps / 100 ) + 35;
		if (getVoitures[j].temps[i] < temps) {
			getVoitures[j].temps[i] = temps;
		}
		getVoitures[j].temps[nombreSecteur] += temps;
	}
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
