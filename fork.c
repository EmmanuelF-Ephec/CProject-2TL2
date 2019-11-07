void forkVoitures(int tab[20], int shmid) {
    int pid_fils;
    int pere = getpid();
    printf("pere %d\n" , pere);
    for (int i = 0; i < 20; i++) {
    
        pid_fils = fork();
        if (pid_fils == 0) {
            printf("Je suis le fils\n");
            struct Voiture *getVoitures;
            if ((getVoitures = shmat(shmid, 0, 0) == -1)) {
                printf("Erreur : shmat\n");
            }
            else {
                //printf("%d\n", tab[i]);
                getVoitures[i].id = tab[i];
                printf("%s\n", getVoitures[i].id);
            }
            exit(0);
        }
        else
        {
            printf("je suis le pere\n");
        }
        
    }
}
