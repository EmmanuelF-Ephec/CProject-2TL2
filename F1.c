#include "library.h"

void wait(char periode[2]) {
    int num;
    printf("Entrez un nombre pour passer à %s\n",periode);
    while (scanf("%d",&num) != 1) {
        getchar();
        printf ("Entrez bien un nombre !\n");
    }
}

int qualif (int ref[],size_t nbrVoiture,int key,int baisse) {
    int shmid;
    struct Voiture *refVoitures;//Référence pour la quantité de mémoire nécessaire à l'alocation de la mé&moire partagée
    if ((shmid = shmget(key, nbrVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget qualif\n");
        return -1;
    };//Initialisation du shmid avec traitement en cas d'erreur
    forkVoitures(ref, shmid, nbrVoiture);

    printf("\n    Place | n°   | Meilleur temps\n");
    for (int i=0;i<(nbrVoiture-baisse);i++) {//récupération des id des 15 meilleures voitures de Q1 et affichage avec leur meilleur temps au tour
        printf("     %d%s| %d%s|   %.2f\n" ,i+1,(i < 9) ? "    " : "   ", classements[i].id, (classements[i].id < 9) ? "    " : "   ", classements[i].meilleursTemps[3]);
    }
}

int main (int argc, char *argv[]) {
    int key = 50; // Clé d'accès à la mémoire partagée
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

    int num;
    printf("Choisissez la taile du circuit (en kilomètres)\n");
    while (scanf("%d",&num) != 1) {
        getchar();
        printf ("Entrez bien un nombre\n");
    }
    tempsSecteur = 4*num;
    
    //clearShm(key);//réinitialisation de toutes les shm's qui seront utilisées au cours de l'exécution du programme
    int voituresQ1[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};//Les voitures inscrites
    int nombreVoiture = sizeof(voituresQ1) / sizeof(voituresQ1[0]);//Calcul du nombre de voiture
    //------------------------------------------P1----------------------------------------------------------
    trigger = 9;
    wait("P1");
    qualif(voituresQ1,nombreVoiture,key,0);
    key+=5;

    //------------------------------------------P2----------------------------------------------------------
    wait("P2");
    qualif(voituresQ1,nombreVoiture,key,0);
    key+=5;

    //------------------------------------------P3----------------------------------------------------------
    wait("P3");
    trigger = 6;
    qualif(voituresQ1,nombreVoiture,key,0);
    key+=5;

    //------------------------------------------Q1----------------------------------------------------------
    trigger = 6;
    wait("Q1");
    qualif(voituresQ1,nombreVoiture,key,5);
    key+=5;

    //------------------------------------------Q2----------------------------------------------------------
    trigger = 5;
    nombreVoiture -= 5;//ajustement du nombre de voitures au départ de Q2
    int voituresQ2[nombreVoiture];
    for (i=0;i<nombreVoiture;i++) {
        voituresQ2[i] = classements[i].id;
    }//Préparation de la prochaine qualif
    wait("Q2");
    qualif(voituresQ2,nombreVoiture,key,5);
    key+=5;

    //------------------------------------------Q3----------------------------------------------------------
    trigger = 4;
    nombreVoiture -= 5;//ajustement du nombre de voitures au départ de Q3
    int voituresQ3[nombreVoiture];
    for (i=0;i<nombreVoiture;i++) {
        voituresQ3[i] = classements[i].id;
    }//Préparation de la prochaine qualif
    wait("Q3");
    qualif(voituresQ3,nombreVoiture,key,0);
    //----------------------------------------Classement Qualifs---------------------------------------------------------
    nombreVoiture = sizeof(voituresQ1) / sizeof(voituresQ1[0]);//Calcul du nombre de voiture
    int course[nombreVoiture];//les voitures inscrites à la course
    printf("\n  Grille de départ de la course :\n");
    printf("\n    Place | n°   | Meilleur temps\n");
    for (i=0;i<nombreVoiture;i++) {//récupération de l'ordre du classement des voitures après les qualifications afin d'afficher la grille de départ et les meilleurs temps au tour respectifs des voitures
        course[i] = classements[i].id;
        printf("     %d%s| %d%s|   %.2f\n" ,i+1,(i < 9) ? "    " : "   ", course[i], (course[i] < 9) ? "    " : "   ", classements[i].meilleursTemps[3]);
    }

    //----------------------------------------Course---------------------------------------------------------
    if ((shmid = shmget(3, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };//Initialisation du shmid avec traitement en cas d'erreur

    //Phase de confirmation
    int newNum;
    printf("Entrez un nombre pour passer à la course");
    while (scanf("%d",&newNum) != 1) {
        getchar();
        printf ("Entrez bien un nombre !\n");
    }

    forkCourse(course, shmid, nombreVoiture, sem_set_id);

    //----------------------------------------Affichage des résultats de la course-----------------------------
        
    printf("\n\n  Résultats de la course :\n");
    printf("\n    Place | n°   | Meilleur temps | Temps total  |\n");
    for (i=0;i<nombreVoiture;i++) {
        printf("     %d%s| %d%s|     %.2f     |    %.2f   %s|\n" ,i+1,(i < 9) ? "    " : "   ", classements[i].id,
        (classements[i].id < 9) ? "    " : "   ", classements[i].meilleursTemps[3], 
        classements[i].tempsTotalCourse, ((classements[i].tempsTotalCourse < 999.99) ? " " : ""));
    }//récupération du classement final dans voitures et affichage du nombre de tours, du temps total et du meilleur temps au tour de chaque voiture
    printf("\n\n\n\n");

    return 0;
}
