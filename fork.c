void forkVoitures(int tab[20], int shm, size_t nombreVoiture) {
    int pid_fils;
    int pere = getpid();

    int shmid = shm;

    int numVoiture, numSecteurs;
    struct Voiture *getVoitures;
        if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
            printf("Erreur : shmat\n");
        }

    for (numVoiture = 0; numVoiture < 5;numVoiture++) {
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };

        if (pid_fils == 0) {
            getVoitures[numVoiture].id = tab[numVoiture];
            tour(3, shmid,numVoiture);
            exit(0);
        }
    }
    for (int cpt = 0;cpt<3;cpt++) {
        if (pid_fils > 0) {
            sleep(3);
            for (int cptVoiture = 0;cptVoiture<5;cptVoiture++) {
                struct Voiture classement[nombreVoiture];
                memcpy(classement, getVoitures, sizeof(classement));
                printf("Voiture n°%d\n", getVoitures[cptVoiture].id );
                for (numSecteurs = 0; numSecteurs < 3; numSecteurs++ ){
                    printf( "Meilleur temps S%d : %.2f secondes\n", (numSecteurs+1), getVoitures[cptVoiture].meilleursTemps[numSecteurs] );
	            }
	            printf( "Meilleur temps au tour : %.2f secondes\n", getVoitures[cptVoiture].meilleursTemps[3] );
            }
        }
    }
}
