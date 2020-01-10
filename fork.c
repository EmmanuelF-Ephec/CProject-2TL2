void forkVoitures(int tab[20], int shmid, size_t nombreVoiture) {
    int pid_fils;//Identification entre processus père et fils
    int pere = getpid();

    int numVoiture, numSecteurs;//Variables de comptage
    struct Voiture *getVoitures;//Adresses d'accès à la mémoire partagée
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
        printf("Erreur : shmat\n");
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        getVoitures[numVoiture].id = tab[numVoiture];//Initialisation en mémoire partagée des numéros de voitures
        /*NB - j'ai retiré l'initialisation de estOut et auStand à zéro car c'est déjà fait dans la fonction clearShm sur F1.c*/
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };//Création d'autant de processus fils que nécessaire(=nombre de voitures) avec traitement en cas d'erreur

        if (pid_fils == 0) {//Si je suis une voiture
            int id = getpid();
            rouler(shmid,numVoiture,id);
            exit(0);
        }
    }
    if (pid_fils > 0) {//Si je suis le père (l'afficheur)...
        int i,j;//Variables de comptage
        struct Voiture c;//Variable intermédiaire pour le tri du classement
        struct Voiture classement[nombreVoiture];//Classement des voitures

        for (compteur = 0;compteur<trigger;compteur++) {//boucle destinée à afficher un nombre fixé de fois les résultats avant de clôturer la scéance de qualification
            sleep(2);
            memcpy(classement, getVoitures, sizeof(classement));//Copie de la mémoire partagée dans le classement (en variable locale)

            for(i=0;i<nombreVoiture;i++) {
                for(j=i+1;j<nombreVoiture;j++) {
                    if ( classement[i].meilleursTemps[3] > classement[j].meilleursTemps[3]) {
                        c = classement[i];
                        classement[i] = classement[j];
                        classement[j] = c;
                    }
                }
            }//Tri du classement

            printf("\n   Affichage n°%d\n",compteur+1);
            printf("\n   Voiture   |   Temps S1    |    Temps S2   |    Temps S3   |   Temps tour   |\n");
            for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {
                printf("     n°%d %s|     %.2f     |     %.2f     |     %.2f     |     %.2f     |\n", 
                classement[cptVoiture].id, ((classement[cptVoiture].id < 10) ? "    " : "   "), classement[cptVoiture].meilleursTemps[0], classement[cptVoiture].meilleursTemps[1], classement[cptVoiture].meilleursTemps[2], classement[cptVoiture].meilleursTemps[3]);
            }//affichage instantané de l'état de la qualification
        }

        for (i = 0;i<nombreVoiture;i++) {
            classements[i].id = classement[i].id;
            classements[i].tempsTotalCourse = classement[i].tempsTotalCourse;
            for (numSecteurs = 0; numSecteurs < 4; numSecteurs++ ){
                classements[i].meilleursTemps[numSecteurs] = classement[i].meilleursTemps[numSecteurs];
	        }
        }//Copie du classement local dans le classement en variable générale
    }
}

void forkCourse(int tab[20], int shmid, size_t nombreVoiture, int sem_set_id){
    int pid_fils;//Identification entre processus père et fils
    int pere = getpid();

    int numVoiture, numSecteurs;//Variables de comptage

    struct Voiture *getVoitures;//Adresses d'accès à la mémoire partagée pour la création des temps
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
        printf("Erreur : shmat\n");
    }

    int shmidInstant;//shmid de la shm
    if ((shmidInstant = shmget(999, nombreVoiture*sizeof(struct Voiture) , IPC_CREAT | 0666 )) == -1) {//récupération du shmid de la shm de gestion instantanée des dépassements
        printf("Erreur : shmget\n");
    };
    struct Voiture *classementsCourseInstant;
	if ((classementsCourseInstant = shmat(shmidInstant, 0, 0)) == NULL) {//adresse d'accès à la shm de gestion instantanée des dépassements (GID)
   		printf("Erreur : shmat\n");	
	}

    for (int numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {//initialisation de la shm GID selon la grille de départ de la course
        classementsCourseInstant[numVoiture].id = tab[numVoiture];
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        getVoitures[numVoiture].id = tab[numVoiture];//Initialisation en mémoire partagée des numéros de voitures
        /* cfr NB de forkVoiture */
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };//Création d'autant de processus fils que nécessaire(=nombre de voitures) avec traitement en cas d'erreur

        if (pid_fils == 0) {//Si je suis une voiture
            int id = getpid();
            //rouler(shmid,numVoiture,id);
            roulerCourse(shmid, numVoiture, id, sem_set_id,shmidInstant);
            exit(0);
        }
    }
    if (pid_fils > 0) {//Si je suis le père (l'afficheur)...
        compteur = 0;//variable de comptage
        int flag = 0;
        while (flag == 0) {
            sleep(2);
            for (int i = 0;i<nombreVoiture;i++) {
                if (classementsCourseInstant[i].nombreTour >= nombreToursCourse) {
                    compteur ++;
                }
            }
            if (compteur == nombreVoiture) {
                flag = 1;
            }
            if (flag == 0 && compteur <= nombreToursCourse) {
                //printf("\n   Classement à l'issue du tour %d\n", (i+1));
                printf("\n   Voiture   |   Temps S1    |    Temps S2   |    Temps S3   |   Temps tour   |   Temps total   |   Tours    |\n");
                for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {
                    printf("     n°%d %s|     %.2f     |     %.2f     |     %.2f     |     %.2f     |     %.2f     %s|     %d     %s|\n", 
                    classementsCourseInstant[cptVoiture].id, ((classementsCourseInstant[cptVoiture].id < 10) ? "    " : "   "), 
                    classementsCourseInstant[cptVoiture].meilleursTemps[0], classementsCourseInstant[cptVoiture].meilleursTemps[1], 
                    classementsCourseInstant[cptVoiture].meilleursTemps[2], classementsCourseInstant[cptVoiture].meilleursTemps[3], 
                    classementsCourseInstant[cptVoiture].tempsTotalCourse, 
                    ((classementsCourseInstant[cptVoiture].tempsTotalCourse < 999.99) ? " " : ""), 
                    classementsCourseInstant[cptVoiture].nombreTour+1, ((classementsCourseInstant[cptVoiture].nombreTour < 10) ? " " : ""));
                }
            }//affichage instantané de l'état de la course
        }
        memcpy(classements, classementsCourseInstant, nombreVoiture*sizeof(struct Voiture));//copie de la shm GID dans le classement (en variable locale)
    }
}
