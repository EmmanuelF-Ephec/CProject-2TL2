int genRandomNbr(int nbrMin, int nbrMax) {
    int randNbr = rand()%(nbrMax + 1 - nbrMin) + nbrMin;
    return randNbr;
}

int crash() { // Probabilité de crash : 1/300 par secteur
	int crash = 0;
	int randNbr = genRandomNbr(0,300);
	if (randNbr < 2) {
		crash = 1;
	}
	return crash;
}

int stand() { // Probabilité de passage au stand : 15%
	int stand = 0;
	int randNbr = genRandomNbr(0,99);
	if (randNbr < 16) {
		stand = 1;
	}
	return stand;
}

void randGen(int numVoiture, int shm, int id){
	double temps;
	double tempsTour;

	struct Voiture *setVoitures;
	if ((setVoitures = shmat(shm, 0, 0)) == NULL) {
   		printf("Erreur : shmat\n");	
	}
	tempsTour = 0;
	/*int numSecteur = 0;
	while (numSecteur < 3 || setVoitures[numVoiture].estOut != 1) {
		sleep(1);
		srand(time(NULL)*setVoitures[numVoiture].id*(numSecteur+1));

		double temps =(double)(genRandomNbr(3499, 4999)/100.00);
		tempsTour += temps;

		if (crash() == 1)  {
			setVoitures[numVoiture].estOut = 1;
		}

		if (numSecteur == 3) {	
			if (stand() == 1) {
				setVoitures[numVoiture].auStand++;
				double tempsArret = (double) (genRandomNbr(199, 399) / 100.00);
				temps += tempsArret;
				tempsTour += tempsArret;
			}
		}

		if (setVoitures[numVoiture].meilleursTemps[numSecteur] > temps || setVoitures[numVoiture].meilleursTemps[numSecteur] == 0) {
			setVoitures[numVoiture].meilleursTemps[numSecteur]  = temps;
		}
		numSecteur++;
	}*/
	for (int numSecteur = 0; numSecteur < 3; numSecteur++ ){
		sleep(1);
		srand(numSecteur*id*setVoitures[numVoiture].id);
		temps = (double)(genRandomNbr(3499, 4999)/100.00);
		tempsTour += temps;
		if (setVoitures[numVoiture].meilleursTemps[numSecteur] > temps || setVoitures[numVoiture].meilleursTemps[numSecteur] == 0) {
			setVoitures[numVoiture].meilleursTemps[numSecteur] = temps;
		}
	}
	if ((setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0) && setVoitures[numVoiture].estOut != 1 ) {
			setVoitures[numVoiture].meilleursTemps[3] = tempsTour;
	}
}

void rouler (int shmid, int numVoiture, int id) {
	struct Voiture *getVoitures;
	if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
   		printf("Erreur : shmat\n");	
	}
	int compteur = 0;
	while (compteur < trigger) {
		randGen(numVoiture, shmid, id);
	}
}
