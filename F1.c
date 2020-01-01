#include "library.h"

void clearShm(int key){
    int nbrDeShmAClear = 3;
    int nombreVoiture = 20;
    int shmid;
    struct Voiture *clearVoitures;
    for(int i = 0; i < nbrDeShmAClear; i++){
        if ((shmid = shmget((key+i), nombreVoiture*sizeof(clearVoitures) , IPC_CREAT | 0666 )) == -1) {
            printf("Erreur : shmget\n");
        };
        if ((clearVoitures = shmat(shmid, 0, 0)) == NULL) {
            printf("Erreur : shmat\n");
        }
        for(int j = 0; j < nombreVoiture; j++){
            for(int k = 0; k < 4; k++){
                clearVoitures[j].meilleursTemps[k] = 0;
            }
        }
        nombreVoiture -= 5;
    }
}

int main (int argc, char *argv[]) {
    int key = 280; // Clé d'accès à la mémoire partagée
    int shmid;//Id pour accèder à la mémoire partagée
    struct Voiture *refVoitures;//Référence pour la quantité de mémoire nécessaire à l'alocation de la mé&moire partagée
    int i;//Variable de comptage 

    clearShm(key);

    //------------------------------------------Q1----------------------------------------------------------
    int voitures[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};//Les voitures inscrites
    size_t nombreVoiture = sizeof(voitures) / sizeof(voitures[0]);//Calcul du nombre de voiture

    if ((shmid = shmget(key, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };//Initialisation du shmid avec traitement en cas d'erreur
    key++;//Augmentation de la clé pour les création de shmid suivantes

    printf("Q1\n");
    forkVoitures(voitures, shmid, nombreVoiture);

    int voituresQ2[15];
    sleep(2);
    for (i=0;i<15;i++) {
        voituresQ2[i] = classements[i].id;
        printf("%d : %d - %.2f\n",i+1,voituresQ2[i],classements[i].meilleursTemps[3]);
    }

    //------------------------------------------Q2----------------------------------------------------------
    nombreVoiture = sizeof(voituresQ2) / sizeof(voituresQ2[0]);
    if ((shmid = shmget(key, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };
    key++;

    printf("Q2\n");
    forkVoitures(voituresQ2, shmid, nombreVoiture);
    int voituresQ3[10];
    sleep(2);
    for (i=0;i<10;i++) {
        voituresQ3[i] = classements[i].id;
        printf("%d : %d - %.2f\n",i+1,voituresQ2[i],classements[i].meilleursTemps[3]);
    }

    //------------------------------------------Q3----------------------------------------------------------
    nombreVoiture = sizeof(voituresQ3) / sizeof(voituresQ3[0]);
    if ((shmid = shmget(key, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };
    key++;

    printf("Q3\n");
    forkVoitures(voituresQ3, shmid, nombreVoiture);

    //------------------------------------Affichage post-qualifications-------------------------------------

    printf("Grille de départ de la course :\n");
    int grille[10];
    for (i=0;i<20;i++) {
        grille[i] = classements[i].id;
        voitures[i] = classements[i].id;
        printf("%d : %d - %.2f\n",i+1,voitures[i],classements[i].meilleursTemps[3]);
    }

    return 0;
}