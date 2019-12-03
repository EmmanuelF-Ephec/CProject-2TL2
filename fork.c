void forkVoitures(int tab[20], int shm, size_t nombreVoiture) {
    int pid_fils;
    int pere = getpid();

    int shmid = shm;

    int i, j;

    for (i = 0; i < nombreVoiture;i++) {
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };

        struct Voiture *getVoitures;
        if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
            printf("Erreur : shmat\n");
        }

        if (pid_fils == 0) {
            getVoitures[i].id = tab[i];
            getVoitures[j].temps[3] = 0;
            tour(3, shmid,i);
            exit(0);
        }
        else {
            sleep(1);
            struct Voiture classement[nombreVoiture];
            memcpy(classement, getVoitures, sizeof(classement));
            for (j = 0; j < 3; j++ ){
                printf( "S%d : %.2f secondes\n", (j+1), getVoitures[i].temps[j] );
	        }
	        printf( "Temps du tour : %.2f secondes\n", getVoitures[i].temps[3] );
        }
    }
    
}
