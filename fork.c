void forkVoitures(int tab[20], int shm) {
    int pid_fils;
    int pere = getpid();

    int shmid = shm;

    int i, j;
    //int lengthTab = sizeof(tab) / sizeof(tab[0]);
    int nombreZone = 3;
    double tempsTour[nombreZone + 1];
    //int lengthTempsTour;

    for (i = 0; i < 3;i++) {
        sleep(1);
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };
        if (pid_fils == 0) {
            struct Voiture *getVoitures;
            if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
                printf("Erreur : shmat\n");
            }
            else {
                getVoitures[i].id = tab[i];
                randGen(i, shmid);
                /*
                for ( j = 0; j < 3; j++ ){
                    printf( "S%d : %.2f secondes\n", (j+1), tempsTour[i] );
                }
                printf( "Temps du tour : %.2f secondes\n", tempsTour[3] );
                */
                
                //lengthTempsTour = sizeof(tempsTour) / sizeof(tempsTour[0]);
                /*for (j = 0;j<4;j++) {
                    getVoitures[i].temps[j] = tempsTour[j];
                    printf("%lf\n", getVoitures[i].temps[j]);
                }
                */

                exit(0);
            }
        }
    }
}
