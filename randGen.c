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

void randGen(int numVoiture, int shm, int id){
	double temps;//récupère un temps aléatoire d'un secteur
	double tempsTour;//additionne les temps aléatoires générés de chaque secteur pour avoir un temps aléatoire au tour

	struct Voiture *setVoitures;
	if ((setVoitures = shmat(shm, 0, 0)) == NULL) {//Adresses d'accès à la mémoire partagée pour la création des temps
   		printf("Erreur : shmat\n");	
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
		temps = (double)(genRandomNbr(3499, 4999)/100.00);
		tempsTour += temps;
		if (setVoitures[numVoiture].meilleursTemps[numSecteur] > temps || setVoitures[numVoiture].meilleursTemps[numSecteur] == 0) {
			setVoitures[numVoiture].meilleursTemps[numSecteur] = temps;
		}//enregistrement des meilleurs temps secteur dans la shm
	}
	if ((setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0) /*&& setVoitures[numVoiture].estOut != 1*/ ) {
			setVoitures[numVoiture].meilleursTemps[3] = tempsTour;
	}//enregistrement du meilleur temps au tour dans la shm
}

void randGenCourse(int numVoiture, int shm, int id, int tour, int sem_set_id, int shmInstant){
	double temps;//récupère un temps aléatoire d'un secteur
	double tempsTour;//additionne les temps aléatoires générés de chaque secteur pour avoir un temps aléatoire au tour

	struct Voiture *voituresIRT;
	if ((voituresIRT = shmat(shm, 0, 0)) == NULL) {//Adresses d'accès à la mémoire partagée pour la création des temps
   		printf("Erreur : shmat\n");	
	}
	struct Voiture *classementsCourseInstant;
	if ((classementsCourseInstant = shmat(shmInstant, 0, 0)) == NULL) {//Adresses d'accès à la shm GID
   		printf("Erreur : shmat\n");	
	}

	tempsTour = 0;//réinitialisation du temps au tour avant de générer des temps aléatoires pour les 3 secteurs
	for (int numSecteur = 0; numSecteur < 3; numSecteur++ ){//génération de temps aléatoires pour les 3 secteurs
		sleep(1);
		//printf("%d - %d - %d",numVoiture,num)
		srand(time(NULL)*getpid()*(numVoiture+1));
		temps = (double)(genRandomNbr(3499, 4999)/100.00);
		tempsTour += temps;
		if (voituresIRT[numVoiture].meilleursTemps[numSecteur] > temps || voituresIRT[numVoiture].meilleursTemps[numSecteur] == 0) {
			voituresIRT[numVoiture].meilleursTemps[numSecteur] = temps;
		}//enregistrement des meilleurs temps secteur dans la shm
	}
	voituresIRT[numVoiture].tempsTotalCourse += tempsTour;//enregistrement tu temps total pour la course
	voituresIRT[numVoiture].nombreTour = tour;//incrémentation du nombre de tours
	if ((voituresIRT[numVoiture].meilleursTemps[3] > tempsTour || voituresIRT[numVoiture].meilleursTemps[3] == 0) /*&& voituresIRT[numVoiture].estOut != 1*/ ) {
			voituresIRT[numVoiture].meilleursTemps[3] = tempsTour;
	}//enregistrement des meilleurs temps secteur dans la shm
	if(tour == nombreToursCourse){//validation dans le cas où la voiture vient d'effectuer son dernier tour (cfr fork.c :: ligne 109)
		voituresIRT[numVoiture].validation = 1;
	}

	struct sembuf sem_op;//structure nécessaire au fonctionnement des sémaphores
	/* Décrémentation de la valeur de la sémaphore afin d'assurer qu'un seul fils ne puisse y accéder à la fois */
	sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);

    for(int i = 0; i<20; i++){//recopiage des valeurs enregistrées dans voitureIRT dans la shm GID pour la voiture concernée
		if(classementsCourseInstant[i].id == voituresIRT[numVoiture].id){//on cherche dans la structure l'emplacement correspondant à l'id de la voiture concernée
			for(int j = 0; j < 4; j++){
				classementsCourseInstant[i].meilleursTemps[j] = voituresIRT[numVoiture].meilleursTemps[j];
			}
			classementsCourseInstant[i].tempsTotalCourse = voituresIRT[numVoiture].tempsTotalCourse;
			classementsCourseInstant[i].nombreTour = voituresIRT[numVoiture].nombreTour;
			break;//une fois l'emplacement adhoc trouvé il n'est pas nécessaire de continuer à comparer l'id avec les emplacements suivants du tableau
		}
	}

	struct Voiture c;//Variable intermédiaire pour le tri du classement
	for(int i=0;i<19;i++) {//tri de la shm GID selon ...
		for(int j=i+1;j<20;j++) {
			if ( classementsCourseInstant[i].nombreTour < classementsCourseInstant[j].nombreTour) {//... une majeure (le nombre de tours parcourus) et ...
				c = classementsCourseInstant[i];
				classementsCourseInstant[i] = classementsCourseInstant[j];
				classementsCourseInstant[j] = c;
			}
			else if ( classementsCourseInstant[i].nombreTour== classementsCourseInstant[j].nombreTour){
				if ( classementsCourseInstant[i].tempsTotalCourse > classementsCourseInstant[j].tempsTotalCourse) {// ... une mineure (le temps nécessaire à parcourir les x tours)
					c = classementsCourseInstant[i];
					classementsCourseInstant[i] = classementsCourseInstant[j];
					classementsCourseInstant[j] = c;
				}
			}
		}
	}//Tri du classement

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
		randGen(numVoiture, shmid, id);
	}
}

void roulerCourse (int shmid, int idVoiture, int id, int sem_set_id, int shmInstant){
	for(int i = 0; i < nombreToursCourse; i++){//fais tourner la voiture le nombre de tours précisé dans struct.h
		randGenCourse(idVoiture, shmid, id, (i+1), sem_set_id, shmInstant);
	}
}