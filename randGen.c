void randGen(int numVoiture, int shm){
	double temps;
	double tempsTour;

	struct Voiture *setVoitures;
	if ((setVoitures = shmat(shm, 0, 0)) == NULL) {
   		printf("Erreur : shmat\n");	
	}
	tempsTour = 0;
	for (int numSecteur = 0; numSecteur < 3; numSecteur++ ){
		sleep(1);
		srand(time(NULL)*setVoitures[numVoiture].id*(numSecteur+1));
		temps = rand() % 1600;
		temps = ( temps / 100 ) + 35;
		tempsTour += temps;
		if (setVoitures[numVoiture].meilleursTemps[numSecteur] > temps || setVoitures[numVoiture].meilleursTemps[numSecteur] == 0) {
			setVoitures[numVoiture].meilleursTemps[numSecteur] = temps;
		}
	}
	if (setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0) {
			setVoitures[numVoiture].meilleursTemps[3] = tempsTour;
	}
}

void rouler (int shmid, int numVoiture) {
	struct Voiture *getVoitures;
	if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
   		printf("Erreur : shmat\n");	
	}
	compteur = 0;
	while (compteur < trigger) {
		randGen(numVoiture, shmid);
	}
}
