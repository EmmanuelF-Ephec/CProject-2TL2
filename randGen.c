//Fonction generant un nombre pseudo-aleatoire
//int nbrMin correspond a la valeur minimum pour la generation
//int nbrMax correspond a la valeur maximum pour la generation
int genRandomNbr(int nbrMin, int nbrMax) {
    int randNbr = rand()%(nbrMax + 1 - nbrMin) + nbrMin;//appel a la fonction systeme rand() pour generer un nombre pseudo-aleatoire
    return randNbr;//Retour du nombre genere
}

//Fonction simulant le crash d'une voiture sur base d'une probabilite (1/300 par secteur)
int crash() {
	int crash = 0;//Initialisation de la variable crash, agissant comme un booleen (1 signifie crash, 0 signifie pas de crash)
	int randNbr = genRandomNbr(0,999);//Appel a la methode genRandomNbr sur randGen.c
	if (randNbr < 3) {//Si le nombre pseudo-aleatoire est plus petit que 2...
		crash = 1;//Changement de la valeur de crash pour 1
	}
	return crash;//Retour de crash
}

//Fonction simulant le passage au stand d'une voiture sur base d'une probabilite (15%)
int stand() {
	int stand = 0;//Initialisation de la variable stand, agissant comme un booleen (1 signifie passage au stand, 0 signifie pas de passage au stand)
	int randNbr = genRandomNbr(0,99);//Appel a la methode genRandomNbr sur randGen.c
	if (randNbr < 16) {//Si le nombre pseudo-aleatoire est plus petit que 16...
		stand = 1;//Changement de la valeur de stand pour 1
	}
	return stand;//Retour de stand
}

//Fonction simulant un tour de circuit pour un processus donne (une voiture)
//int numVoiture correspond a l'indice (l'emplacement) de la voiture dans la memoire partagee associee a la sceance en cours
//int shm correspond a l'adresse d'acces a la memoire partagee associee a la sceance en cours
//int id correspond a l'id du processus en cours d'execution
//int sem_set_id correspond a l'adresse d'acces aux semaphore
int randGen(int numVoiture, int shm, int id, int sem_set_id){
	double temps;//Variable qui recuperera un temps aleatoire d'un secteur
	double tempsTour;//Variable qui additionnera les temps aleatoires generes de chaque secteur pour avoir un temps aleatoire au tour

	struct Voiture *setVoitures;//Pointeur qui contiendra l'adresse d'acces a la memoire partagee associee a la sceance en cours
	if ((setVoitures = shmat(shm, 0, 0)) == NULL) {//Recuperation de l'adresse d'acces a la memoire partagee et...
   		printf("Erreur : shmat randgen voiture\n");//traitement en cas d'erreur
		return -1;
	}

	/*struct sembuf sem_op;//structure necessaire au fonctionnement des semaphores*/
	/* Decrementation de la valeur de la semaphore afin d'assurer qu'un seul fils ne puisse y acceder a la fois */
	/*sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);*/

	tempsTour = 0;//Reinitialisation du temps au tour avant de generer des temps aleatoires pour les 3 secteurs
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
	for (int numSecteur = 0; numSecteur < 3; numSecteur++ ){//Boucle destinee a se repeter autant de fois qu'il y a de secteurs sur le circuit
		sleep(1);//Le processus en cours d'execution marque une petite pause avant de reprendre l'execution
		srand(time(NULL)*getpid()*(numVoiture+1));//definition d'une seed unique pour la generation pseudo-aleatoire a l'aide de rand()
		if (crash() == 1) {//Appel a la methode crash sur randGen.c. Si le retour est 1...
			setVoitures[numVoiture].estOut = 1;//Mise a 1 du champ estOut de la voiture
			tempsTour = 3*(tempsSecteur+5);//Mise du temps tour a la valeur max
			temps = (tempsSecteur+5);//Mise du temps section a la valeur max
			for(int i = 0; i < 3; i++){
				if (setVoitures[numVoiture].meilleursTemps[i] > temps || setVoitures[numVoiture].meilleursTemps[i] == 0) {//Si le temps genere est meilleur (plus petit) que le temps enregistre en memoire partagee...
					setVoitures[numVoiture].meilleursTemps[i] = temps;//remplacement du temps enregistre en memoire partagee par le nouveau temps
				}
			}
			if ((setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0)) {//Si le temps tour genere est meilleur (plus petit) que le temps tour enregistre en memoire partagee...
				setVoitures[numVoiture].meilleursTemps[3] = tempsTour;//remplacement du temps tour enregistre en memoire partagee par le nouveau temps tour
			}
			return 0;//Sortie de la fonction, c'est a dire que le tour est termine
		}
		else {//Sinon...
			temps = (double)(genRandomNbr((tempsSecteur-5)*100, (tempsSecteur+5)*100)/100.00);//Appel a la methode genRandNbr sur randGen.c et modifications de la valeur recuperer afin d'obtenir des nombres reels a deux decimales
			if (numSecteur == 2 && stand() == 1) {//Si la voiture en est a son dernier secteur et que l'appel a la methode stand (sur randGen.c) retourne 1...
				double tempsArret = (double) (genRandomNbr(199,399) / 100.00);//Appel a la methode genRandNbr sur randGen.c afin de generer un temps de passage au stand
				temps += tempsArret;//Ajout de ce temps au temps secteur
				setVoitures[numVoiture].auStand++;//Incrementation du champ auStand de la voiture
			}
			tempsTour += temps;//Somme des temps generes au fur et a mesure
			if (setVoitures[numVoiture].meilleursTemps[numSecteur] > temps || setVoitures[numVoiture].meilleursTemps[numSecteur] == 0) {//Si le temps genere est meilleur (plus petit) que le temps enregistre en memoire partagee...
				setVoitures[numVoiture].meilleursTemps[numSecteur] = temps;//remplacement du temps enregistre en memoire partagee par le nouveau temps
			}
		}
	}
	if ((setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0 && setVoitures[numVoiture].estOut != 1 )) {//Si le temps tour genere est meilleur (plus petit) que le temps tour enregistre en memoire partagee...
		setVoitures[numVoiture].meilleursTemps[3] = tempsTour;//remplacement du temps tour enregistre en memoire partagee par le nouveau temps tour
	}

	/*int dt_return;//Variable pour recuperer le retour de shmdt()
	if ((dt_return = shmdt(setVoitures)) == -1){//Detachement de la memoire partagee associee a la sceance en cours
		printf("Erreur : shmdt\n");//traitement en cas d'erreur
	}*/

	/* Retablissement de la valeur initiale de la semaphore afin de permettre au fils suivant d'y acceder */
	/*sem_op.sem_num = 0;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
	sleep(1);//Le processus en cours d'execution marque une petite pause avant de reprendre l'execution*/
}


//Fonction simulant un tour de circuit pour un processus donne (une voiture) lors de la course
//int numVoiture correspond a l'indice (l'emplacement) de la voiture dans la memoire partagee associee a la course
//int shm correspond a l'adresse d'acces a la memoire partagee associee a la course
//int id correspond a l'id du processus en cours d'execution
//int tour correspond au numero du tour de la voiture
//int sem_set_id correspond a l'adresse d'acces aux semaphore
int randGenCourse(int numVoiture, int shm, int id, int tour, int sem_set_id){
	double temps;//Variable qui recuperera un temps aleatoire d'un secteur
	double tempsTour;//Variable qui additionnera les temps aleatoires generes de chaque secteur pour avoir un temps aleatoire au tour
	

	struct Voiture *setVoitures;//Pointeur qui contiendra l'adresse d'acces a la memoire partagee associee a la sceance en cours
	if ((setVoitures = shmat(shm, 0, 0)) == NULL) {//Recuperation de l'adresse d'acces a la memoire partagee et...
   		printf("Erreur : shmat randgencourse voiture\n");//traitement en cas d'erreur
		return -1;
	}

	struct sembuf sem_op;//structure necessaire au fonctionnement des semaphores
	/* Decrementation de la valeur de la semaphore afin d'assurer qu'un seul fils ne puisse y acceder a la fois */
	sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);

	tempsTour = 0;//Reinitialisation du temps au tour avant de generer des temps aleatoires pour les 3 secteurs
	for (int numSecteur = 0; numSecteur < 3; numSecteur++ ){//Boucle destinee a se repeter autant de fois qu'il y a de secteurs sur le circuit
		sleep(1);//Le processus en cours d'execution marque une petite pause avant de reprendre l'execution
		srand(time(NULL)*getpid()*(numVoiture+1));//definition d'une seed unique pour la generation pseudo-aleatoire a l'aide de rand()
		if (crash() == 1) {//Appel a la methode crash sur randGen.c. Si le retour est 1...
			setVoitures[numVoiture].estOut = 1;//Mise a 1 du champ estOut de la voiture
			tempsTour = 3*(tempsSecteur+5);//Mise du temps tour a la valeur max
			temps = (tempsSecteur+5);//Mise du temps section a la valeur max
			for(int i = 0; i < 3; i++){
				if (setVoitures[numVoiture].meilleursTemps[i] > temps || setVoitures[numVoiture].meilleursTemps[i] == 0) {//Si le temps genere est meilleur (plus petit) que le temps enregistre en memoire partagee...
					setVoitures[numVoiture].meilleursTemps[i] = temps;//remplacement du temps enregistre en memoire partagee par le nouveau temps
				}
			}
			if ((setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0)) {//Si le temps tour genere est meilleur (plus petit) que le temps tour enregistre en memoire partagee...
				setVoitures[numVoiture].meilleursTemps[3] = tempsTour;//remplacement du temps tour enregistre en memoire partagee par le nouveau temps tour
			}
			return 0;//Sortie de la fonction, c'est a dire que le tour est termine
		}
		else {//Sinon...
			temps = (double)(genRandomNbr((tempsSecteur-5)*100, (tempsSecteur+5)*100)/100.00);//Appel a la methode genRandNbr sur randGen.c et modifications de la valeur recuperer afin d'obtenir des nombres reels a deux decimales
			if (numSecteur == 2 && stand() == 1) {//Si la voiture en est a son dernier secteur et que l'appel a la methode stand (sur randGen.c) retourne 1...
				double tempsArret = (double) (genRandomNbr(199,399) / 100.00);//Appel a la methode genRandNbr sur randGen.c afin de generer un temps de passage au stand
				temps += tempsArret;//Ajout de ce temps au temps secteur
				setVoitures[numVoiture].auStand++;//Incrementation du champ auStand de la voiture
			}
			tempsTour += temps;//Somme des temps generes au fur et a mesure
			if (setVoitures[numVoiture].meilleursTemps[numSecteur] > temps || setVoitures[numVoiture].meilleursTemps[numSecteur] == 0) {//Si le temps genere est meilleur (plus petit) que le temps enregistre en memoire partagee...
				setVoitures[numVoiture].meilleursTemps[numSecteur] = temps;//remplacement du temps enregistre en memoire partagee par le nouveau temps
			}
		}
	}
	if(setVoitures[numVoiture].estOut != 1){//sécurité permettant d'éviter que les voitures out continuent d'incrémenter leurs tours et leur temps total
		setVoitures[numVoiture].tempsTotalCourse += tempsTour;//Prise en compte du nouveau temps tour dans le temps total de la course
		setVoitures[numVoiture].nombreTour = tour;//Incrementation du nombre de tours
	}
	if ((setVoitures[numVoiture].meilleursTemps[3] > tempsTour || setVoitures[numVoiture].meilleursTemps[3] == 0) /*&& setVoitures[numVoiture].estOut != 1*/ ) {//Si le temps tour genere est meilleur (plus petit) que le temps tour enregistre en memoire partagee...
		setVoitures[numVoiture].meilleursTemps[3] = tempsTour;//remplacement du temps tour enregistre en memoire partagee par le nouveau temps tour
	}

	/*int dt_return;//Variable pour recuperer le retour de shmdt()
	if ((dt_return = shmdt(setVoitures)) == -1){//Detachement de la memoire partagee associee a la sceance en cours
		printf("Erreur : shmdt\n");//traitement en cas d'erreur
	}*/

	/* Retablissement de la valeur initiale de la semaphore afin de permettre au fils suivant d'y acceder */
	sem_op.sem_num = 0;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
	sleep(1);//Le processus en cours d'execution marque une petite pause avant de reprendre l'execution
}

//Fonction permettant de faire faire des tours aux voitures tant que le nombre de rafraichissement de l'affichage requis n'a pas ete atteint
//int shmid correspond a l'adresse d'acces a la memoire partagee associee a la sceance en cours
//int numVoiture correspond a l'indice (l'emplacement) de la voiture dans la memoire partagee associee a la sceance en cours
//int id correspond a l'id du processus en cours d'execution
//int sem_set_id correspond a l'adresse d'acces aux semaphore
void rouler (int shmid, int numVoiture, int id, int sem_set_id) {
	int compteur = 0;//Variable de comptage
	while (compteur < trigger) {//Fais tourner la voiture tant que le nombre de raffraîchissement de l'affichage (precise dans struct.h) n'a pas ete effectue
		if(randGen(numVoiture, shmid, id, sem_set_id) == 0) {//Si un crash a eu lieu lors du tour...
			break;//Sortie de la boucle : la voiture ne tournera plus jusqu'a la fin de la sceance en cours
		}
	}
}

//Fonction permettant de faire faire des tours aux voitures tant que le nombre de tours requis n'a pas ete atteint
//int shmid correspond a l'adresse d'acces a la memoire partagee associee a la course
//int idVoiture correspond a l'indice (l'emplacement) de la voiture dans la memoire partagee associee a la course
//int id correspond a l'id du processus en cours d'execution
//int sem_set_id correspond a l'adresse d'acces aux semaphore
	void roulerCourse (int shmid, int idVoiture, int id, int sem_set_id){
	for(int i = 0; i < nombreToursCourse; i++){//fais tourner la voiture le nombre de tours precise dans struct.h
		if (randGenCourse(idVoiture, shmid, id, (i+1), sem_set_id == 0)) {//Si un crash a eu lieu lors du tour...
			break;//Sortie de la boucle : la voiture ne tournera plus jusqu'a la fin de la sceance en cours
		}
	}
}