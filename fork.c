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
        getVoitures[numVoiture].auStand = 0;
        getVoitures[numVoiture].estOut = 0;
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

        for (compteur = 0;compteur<trigger;compteur++) {
            sleep(1);
            //system("clear");
            memcpy(classement, getVoitures, nombreVoiture*sizeof(struct Voiture));//Copie de la mémoire partagée dans le classement (en variable locale)

            for(i=0;i<nombreVoiture;i++) {
                for(j=i+1;j<nombreVoiture;j++) {
                    if ( classement[i].meilleursTemps[3] > classement[j].meilleursTemps[3]) {
                        c = classement[i];
                        classement[i] = classement[j];
                        classement[j] = c;
                    }
                }
            }//Tri du classement

            printf("Affichage n°%d\n",compteur+1);
            for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {
                printf("Voiture n°%d\n", classement[cptVoiture].id );
                for (numSecteurs = 0; numSecteurs < 3; numSecteurs++ ){
                    printf( "Meilleur temps S%d : %.2f secondes\n", (numSecteurs+1), classement[cptVoiture].meilleursTemps[numSecteurs] );
	            }
	            printf( "Meilleur temps au tour : %.2f secondes\n", classement[cptVoiture].meilleursTemps[3] );
            }
        }
        for (i = 0;i<nombreVoiture;i++) {
            classements[i].id = classement[i].id;
            for (numSecteurs = 0; numSecteurs < 4; numSecteurs++ ){
                classements[i].meilleursTemps[numSecteurs] = classement[i].meilleursTemps[numSecteurs];
	        }
        }//Copie du classement local dans le classement en variable générale
    }
}
