void forkVoitures(int tab[20], int shm) {
    int pid_fils;
    int pere = getpid();

    int shmid = shm;

    int i, j;
    //int lengthTab = sizeof(tab) / sizeof(tab[0]);
    //int lengthTempsTour;

    for (i = 0; i < 20;i++) {
        sleep(1);
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };

        struct Voiture *getVoitures;
        if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
            printf("Erreur : shmat\n");
        }

        if (pid_fils == 0) {
            getVoitures[i].id = tab[i];
            randGen(i, shmid);
            exit(0);
        }
    }
}
