#include "library.h"

void clearShm(int key){
    int nbrDeShmAClear = 3;//correspond aux 3 shm utilisées lors des qualifications
    int nombreVoiture = 20;//correspond au nombre initial de voitures lors des qualifications
    int shmid;//id pour accéder à la mémoire partagée
    struct Voiture *clearVoitures;//représente la shm qui nécéssite d'être réinitialisée

    if ((shmid = shmget((key+nbrDeShmAClear), nombreVoiture*sizeof(clearVoitures) , IPC_CREAT | 0666 )) == -1) {//récupération du shmid de la shm associée à (key+nbrDeShmAClear) (course)
        printf("Erreur : shmget\n");
    };
    if ((clearVoitures = shmat(shmid, 0, 0)) == NULL) {//récupération de la shm dans la structure clearVoitures
        printf("Erreur : shmat\n");
    }
    for(int i = 0; i < nombreVoiture; i++){//réinitialisation des valeurs introduites dans la shm
        for(int j = 0; j < 4; j++){
            clearVoitures[i].meilleursTemps[j] = 0;
        }
        clearVoitures[i].tempsTotalCourse = 0;
        clearVoitures[i].auStand = 0;
        clearVoitures[i].estOut = 0;
        clearVoitures[i].nombreTour = 0;
        clearVoitures[i].id = 0;
        clearVoitures[i].validation = 0;
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
}

int main (int argc, char *argv[]) {
    int key = 280; // Clé d'accès à la mémoire partagée
    int shmid;//Id pour accèder à la mémoire partagée
    struct Voiture *refVoitures;//Référence pour la quantité de mémoire nécessaire à l'alocation de la mé&moire partagée
    int i;//Variable de comptage

    int sem_set_id;//id pour accéder aux sémaphores
    union semun sem_val;//valeur de la sémaphore
    int SEM_ID = 250;//clé d'accès aux sémaphores
    
    if ((sem_set_id = semget(SEM_ID, 1, IPC_CREAT | 0600)) == -1) {//création d'un set de sémaphores avec la clé DEM_ID, ne contient qu'une sémaphore
	    perror("main: semget");
	    exit(1);
    }

    sem_val.val = 1;
    if (semctl(sem_set_id, 0, SETVAL, sem_val) == -1) {//initialisation de la sémaphore à 1
	    perror("main: semctl");
	    exit(1);
    }

    clearShm(key);//réinitialisation de toutes les shm's qui seront utilisées au cours de l'exécution du programme

    //------------------------------------------Q1----------------------------------------------------------
    int voituresQ1[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};//Les voitures inscrites
    size_t nombreVoiture = sizeof(voituresQ1) / sizeof(voituresQ1[0]);//Calcul du nombre de voiture

    if ((shmid = shmget(key, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };//Initialisation du shmid avec traitement en cas d'erreur
    key++;//Augmentation de la clé pour les création de shmid suivantes

    printf("Q1\n");
    forkVoitures(voituresQ1, shmid, nombreVoiture);

    //------------------------------------------Q2----------------------------------------------------------

    int voituresQ2[15];//les voitures encore en lisse après Q1
    sleep(2);
    for (i=0;i<15;i++) {//récupération des id des 15 meilleures voitures de Q1 et affichage avec leur meilleur temps au tour
        voituresQ2[i] = classements[i].id;
        printf("%d : %d - %.2f\n",i+1,voituresQ2[i],classements[i].meilleursTemps[3]);
    }

    nombreVoiture = sizeof(voituresQ2) / sizeof(voituresQ2[0]);//ajustement du nombre de voitures au départ de Q2
    if ((shmid = shmget(key, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };//Initialisation du shmid avec traitement en cas d'erreur
    key++;//Augmentation de la clé pour les création de shmid suivantes

    printf("Q2\n");
    forkVoitures(voituresQ2, shmid, nombreVoiture);

    //------------------------------------------Q3----------------------------------------------------------

    int voituresQ3[10];//les voitures encore en lisse après Q2
    sleep(2);
    for (i=0;i<10;i++) {//récupération des id des 10 meilleures voitures de Q1 et affichage avec leur meilleur temps au tour
        voituresQ3[i] = classements[i].id;
        printf("%d : %d - %.2f\n",i+1,voituresQ2[i],classements[i].meilleursTemps[3]);
    }

    nombreVoiture = sizeof(voituresQ3) / sizeof(voituresQ3[0]);//ajustement du nombre de voitures au départ de Q3
    if ((shmid = shmget(key, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };//Initialisation du shmid avec traitement en cas d'erreur
    key++;//Augmentation de la clé pour les création de shmid suivantes

    printf("Q3\n");
    forkVoitures(voituresQ3, shmid, nombreVoiture);

    //----------------------------------------Course---------------------------------------------------------

    printf("Grille de départ de la course :\n");
    int voitures[20];//les voitures inscrites à la course
    for (i=0;i<20;i++) {//récupération de l'ordre du classement des voitures après les qualifications afin d'afficher la grille de départ et les meilleurs temps au tour respectifs des voitures
        voitures[i] = classements[i].id;
        printf("%d : %d - %.2f\n",i+1,voitures[i],classements[i].meilleursTemps[3]);
    }

    sleep(2);

    nombreVoiture = sizeof(voitures) / sizeof(voitures[0]);//ajustement du nombre de voitures au départ de la course
    if ((shmid = shmget(key, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };//Initialisation du shmid avec traitement en cas d'erreur

    printf("Course :\n");
    forkCourse(voitures, shmid, nombreVoiture, sem_set_id);

    //----------------------------------------Affichage des résultats de la course-----------------------------

    printf("Résultats de la course :\n");
    voitures[20];
    for (i=0;i<20;i++) {//récupération du classement final dans voitures et affichage du nombre de tours, du temps total et du meilleur temps au tour de chaque voiture
        voitures[i] = classements[i].id;
        printf("%d : Voiture %d - %d tours - Temps total : %.2f s - Meilleur tour : %.2f s\n",i+1,voitures[i],classements[i].nombreTour, classements[i].tempsTotalCourse, classements[i].meilleursTemps[3]);
    }

    return 0;
}