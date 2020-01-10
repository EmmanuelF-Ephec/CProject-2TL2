/*F1
    int clearShm(int key){
    int nbrDeShmAClear = 3;//correspond aux 3 shm utilisées lors des qualifications
    int nombreVoiture = 20;//correspond au nombre initial de voitures lors des qualifications
    int shmid;//id pour accéder à la mémoire partagée
    struct Voiture *clearVoitures;//représente la shm qui nécéssite d'être réinitialisée
    int shmidInstant;//shmid de la shm GID
    struct Voiture *clearCourseInstant;//représente la shm GID qui nécéssite d'être réinitialisée

    if ((shmidInstant = shmget(2, nombreVoiture*sizeof(struct Voiture) , IPC_CREAT | 0666 )) == -1) {//récupération du shmid de la shm de gestion instantanée des dépassements
        printf("Erreur : shmget\n");
        return -1;
    };
	if ((clearCourseInstant = shmat(shmidInstant, 0, 0)) == NULL) {//adresse d'accès à la shm de gestion instantanée des dépassements (GID)
   		printf("Erreur : shmat\n");
        return -1;
	}
    if ((shmid = shmget((key+nbrDeShmAClear), nombreVoiture*sizeof(clearVoitures) , IPC_CREAT | 0666 )) == -1) {//récupération du shmid de la shm associée à (key+nbrDeShmAClear) (course)
        printf("Erreur : shmget\n");
        return -1;
    };
    if ((clearVoitures = shmat(shmid, 0, 0)) == NULL) {//récupération de la shm dans la structure clearVoitures
        printf("Erreur : shmat\n");
        return -1;
    }

    for(int i = 0; i < nombreVoiture; i++){//réinitialisation des valeurs introduites dans la shm
        for(int j = 0; j < 4; j++){
            clearCourseInstant[i].meilleursTemps[j] = 0;
        }
        clearCourseInstant[i].tempsTotalCourse = 0;
        clearCourseInstant[i].auStand = 0;
        clearCourseInstant[i].estOut = 0;
        clearCourseInstant[i].nombreTour = 0;
        clearCourseInstant[i].id = 0;
        clearCourseInstant[i].validation = 0;
    }
    
    for(int i = 0; i < nbrDeShmAClear; i++){
        if ((shmid = shmget((key+i), nombreVoiture*sizeof(clearVoitures) , IPC_CREAT | 0666 )) == -1) {//récupération du shmid de la shm associée à (key+i) (Q1, Q2 et Q3)
            printf("Erreur : shmget\n");
        };
        if ((clearVoitures = shmat(shmid, 0, 0)) == NULL) {//récupération de la shm dans la structure clearVoitures
            printf("Erreur : shmat\n");
        }
        for(int j = 0; j < nombreVoiture; j++){//réinitialisation des valeurs introduites dans la shm
            for(int k = 0; k < 4; k++){
                clearVoitures[j].meilleursTemps[k] = 0;
            }
            clearVoitures[j].tempsTotalCourse = 0;
            clearVoitures[i].auStand = 0;
            clearVoitures[i].estOut = 0;
            clearVoitures[j].nombreTour = 0;
            clearVoitures[j].id = 0;
            clearVoitures[i].validation = 0;
        }
        nombreVoiture -= 5;//décrémentation du nombre de voitures (5 voitures éliminées lors des Q1 et Q2)
    }
}*/