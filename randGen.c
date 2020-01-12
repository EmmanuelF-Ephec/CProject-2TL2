int genRandomNbr(int nbrMin, int nbrMax) {//génération d'un nombre aléatoire compris entre nbrMin et nbrMax
    int randNbr = rand()%(nbrMax + 1 - nbrMin) + nbrMin;
    return randNbr;
}

int crash() {//Probabilité de crash : 1/300 par secteur
	int crash = 0;
	int randNbr = genRandomNbr(0,300);
	if (randNbr < 2) {
		crash = 1;
	}
	return crash;
}

int stand() {//Probabilité de passage au stand : 15%
	int stand = 0;
	int randNbr = genRandomNbr(0,99);
	if (randNbr < 16) {
		stand = 1;
	}
	return stand;
}

int randGen(int numVoiture, int shm, int id){
	double temps;//récupère un temps aléatoire d'un secteur
	double tempsTour;//additionne les temps aléatoires générés de chaque secteur pour avoir un temps aléatoire au tour

	struct Voiture *setVoitures;
	if ((setVoitures = shmat(shm, 0, 0)) == NULL) {//Adresses d'accès à la mémoire partagée pour la création des temps
   		printf("Erreur : shmat randgen voiture\n");
		return -1;
	}
	tempsTour = 0;//réinitialisation du temps au tour avant de générer des temps aléatoires pour les 3 secteurs
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
	for (int numSecteur = 0; numSecteur < 3; numSecteur++ ){//génération de temps aléatoires pour les 3 secteurs
		sleep(1);
		srand(time(NULL)*getpid()*(numVoiture+1));
		if (crash() == 1) {
			setVoitures[numVoiture].estOut = 1;
			return 0;
		}
		else {
			temps = (double)(genRandomNbr((tempsSecteur-5)*100, (tempsSecteur+5)*100)/100.00);
			if (numSecteur == 2 && stand() == 1) {
				double tempsArret = (double) (genRandomNbr(199,399) / 100.00);
				temps += tempsArret;
				setVoitures[numVoiture].auStand++;	
			}
		temps = (double)(genRandomNbr((tempsSecteur-5)*100, (tempsSecteur+5)*100)/100.00);
		tempsTour += temps;
		if (setVoitures[numVoiture].meilleursTemps[numSecteur] > temps || setVoitures[numVoiture].meilleursTemps[numSecteur] == 0) {
			setVoitures[numVoiture].meilleursTemps[numSecteur] = temps;
		}//enregistrement des meilleurs temps secteur dans la shm
		}
	}
	if ((setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0) /*&& setVoitures[numVoiture].estOut != 1*/ ) {
		setVoitures[numVoiture].meilleursTemps[3] = tempsTour;
	}//enregistrement du meilleur temps au tour dans la shm
}

int randGenCourse(int numVoiture, int shm, int id, int tour, int sem_set_id){
	double temps;//récupère un temps aléatoire d'un secteur
	double tempsTour;//additionne les temps aléatoires générés de chaque secteur pour avoir un temps aléatoire au tour
	

	struct Voiture *voituresIRT;
	if ((voituresIRT = shmat(shm, 0, 0)) == NULL) {//Adresses d'accès à la mémoire partagée pour la création des temps
   		printf("Erreur : shmat randgencourse voiture\n");	
		return -1;
	}

	tempsTour = 0;//réinitialisation du temps au tour avant de générer des temps aléatoires pour les 3 secteurs
	for (int numSecteur = 0; numSecteur < 3; numSecteur++ ){//génération de temps aléatoires pour les 3 secteurs
		sleep(1);
		//printf("%d - %d - %d",numVoiture,num)
		srand(time(NULL)*getpid()*(numVoiture+1));
		if (crash() == 1) {
			voituresIRT[numVoiture].estOut = 1;
			return 0;
		}
		else {
			temps = (double)(genRandomNbr((tempsSecteur-5)*100, (tempsSecteur+5)*100)/100.00);
			if (numSecteur == 2 && stand() == 1) {
				double tempsArret = (double) (genRandomNbr(199,399) / 100.00);
				temps += tempsArret;
				voituresIRT[numVoiture].auStand++;	
			}
			temps = (double)(genRandomNbr((tempsSecteur-5)*100, (tempsSecteur+5)*100)/100.00);
			tempsTour += temps;
			if (voituresIRT[numVoiture].meilleursTemps[numSecteur] > temps || voituresIRT[numVoiture].meilleursTemps[numSecteur] == 0) {
				voituresIRT[numVoiture].meilleursTemps[numSecteur] = temps;
			}//enregistrement des meilleurs temps secteur dans la shm
		}
	}
	voituresIRT[numVoiture].tempsTotalCourse += tempsTour;//enregistrement tu temps total pour la course
	voituresIRT[numVoiture].nombreTour = tour;//incrémentation du nombre de tours
	if ((voituresIRT[numVoiture].meilleursTemps[3] > tempsTour || voituresIRT[numVoiture].meilleursTemps[3] == 0) /*&& voituresIRT[numVoiture].estOut != 1*/ ) {
		voituresIRT[numVoiture].meilleursTemps[3] = tempsTour;
	}//enregistrement des meilleurs temps secteur dans la shm

	struct sembuf sem_op;//structure nécessaire au fonctionnement des sémaphores
	/* Décrémentation de la valeur de la sémaphore afin d'assurer qu'un seul fils ne puisse y accéder à la fois */
	sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);

	/* Rétablissement de la valeur initiale de la sémaphore afin de permettre au fils suivant d'y accéder */
	sem_op.sem_num = 0;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
	sleep(1);
}

void rouler (int shmid, int numVoiture, int id) {
	int compteur = 0;
	while (compteur < trigger) {//fais tourner la voiture tant que le nombre de raffraîchissement de l'affichage (précisé dans struct.h) n'a pas été effectué
		if(randGen(numVoiture, shmid, id) == 0) {// Si crash
			break;
		}
	}
}

//void roulerCourse (int shmid, int idVoiture, int id, int sem_set_id, int shmInstant){
	void roulerCourse (int shmid, int idVoiture, int id, int sem_set_id){
	for(int i = 0; i < nombreToursCourse; i++){//fais tourner la voiture le nombre de tours précisé dans struct.h
		if (randGenCourse(idVoiture, shmid, id, (i+1), sem_set_id == 0)) { // Si crash
			break;
		}
	}
}