void forkVoitures(int tab[20], int shmid) {
    int pid_fils;
    int pere = getpid();
    int shmid = shmid;
    printf("pere %d\n" , pere);
    for (int i = 0; i < 20; i++) {
    
        pid_fils = fork();
        if (pid_fils == 0) {
            struct Voiture *getVoitures;
            if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
                printf("Erreur : shmat\n");
            }
            else {
                getVoitures[i].id = tab[i];
                getVoitures[i].
            }
            exit(0);
        }
        else
        {
            //printf("je suis le pere\n");
        }
        
    }
}
