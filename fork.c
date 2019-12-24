void forkVoitures(int tab[20], int shm, size_t nombreVoiture) {
    int pid_fils;
    int pere = getpid();

    int shmid = shm;

    int numVoiture, numSecteurs;
    struct Voiture *getVoitures;
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
        printf("Erreur : shmat\n");
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        getVoitures[numVoiture].id = tab[numVoiture];
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };

        if (pid_fils == 0) {//Si je suis une voiture
            rouler(shmid,numVoiture);
            exit(0);
        }
    }
    if (pid_fils > 0) {//Si je suis le père (l'afficheur)
        int i,j;
        struct Voiture c;
        struct Voiture classement[nombreVoiture];

        for (compteur = 0;compteur<trigger;compteur++) {
            sleep(2);
            memcpy(classement, getVoitures, nombreVoiture*sizeof(struct Voiture));

            for(i=0;i<nombreVoiture-1;i++) {
                    for(j=i+1;j<nombreVoiture;j++) {
                        if ( classement[i].meilleursTemps[3] > classement[j].meilleursTemps[3]) {
                            c = classement[i];
                            classement[i] = classement[j];
                            classement[j] = c;
                        }
                    }
            }
            printf("Affichage n°%d\n",compteur+1);
            for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {
                printf("Voiture n°%d\n", classement[cptVoiture].id );
                for (numSecteurs = 0; numSecteurs < 3; numSecteurs++ ){
                    printf( "Meilleur temps S%d : %.2f secondes\n", (numSecteurs+1), classement[cptVoiture].meilleursTemps[numSecteurs] );
	            }
	            printf( "Meilleur temps au tour : %.2f secondes\n", classement[cptVoiture].meilleursTemps[3] );
            }
        }
    }
}
