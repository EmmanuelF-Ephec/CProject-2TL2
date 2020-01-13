#define _GNU_SOURCE//Permet l'utilisation de asprintf()
#include "library.h"//Permet de lier notre fichier principal (F1.c) a la librairie (library.h) et donc a tous les autres fichiers

//Fonction permettant de faire une pause apres l'execution de chaque partie du programme (P1, P2, P3, Q1, Q2, Q3)
//char periode correspond a une chaine de caracteres representant la sceance en cours (P1, P2, P3, Q1, Q2, Q3)
void wait(char periode[2]) {
    int num;//Variable recuperant la valeur entree par l'utilisateur
    printf("Entrez un nombre pour passer a %s\n",periode);//Demande a l'utilisateur d'entrer un nombre afin de continuer l'execution du programme
    while (scanf("%d",&num) != 1) {//Verifie la valeur entree par l'utilisateur : tant qu'il ne s'agit pas d'un nombre...
        getchar();//supprime le caractere errone entre par l'utilisateur...
        printf ("Entrez bien un nombre !\n");//redemande a l'utilisateur d'entrer un nombre
    }
}

//Fonction donnant le depart des voitures pour les sceances d'essai et de qualifications
//Une fois la sceance terminee, elle affiche les resultats
//int ref est un tableau comprenant la liste des numeros des voitures en lisse pour la sceance sur le point d'etre lancee
//size_t nbrVoiture correspond a la taille de ref
//int key correspond a la cle qui sera utilisee pour initialiser la memoire partagee associee a la sceance sur le point d'etre lancee
//int baisse correspond au nombre de voitures qui seront eliminees a l'issue de la sceance sur le point d'etre lancee
//int sem_set_id correspond a l'id d'acces aux semaphores
int qualif (int ref[],size_t nbrVoiture,int key,int baisse, int sem_set_id) {
    int shmid;//Variable pour contenir l'id d'acces a la memoire partagee
    struct Voiture *refVoitures;//Reference pour la quantite de memoire necessaire a l'alocation de la memoire partagee
    if ((shmid = shmget(key, nbrVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {//initialisation de la memoire partagee a l'aide de key, memorisation de l'id d'acces dans shmid et...
        printf("Erreur : shmget qualif\n");//Traitement en cas d'erreur
        return -1;
    };
    forkVoitures(ref, shmid, nbrVoiture, sem_set_id);//Appel de la methode forkVoiture() sur fork.c
    printf("\n    Place | n°   | Meilleur temps\n");//Ecriture des noms des colonnes de l'affichage
    for (int i=0;i<(nbrVoiture-baisse);i++) {//Recuperation des id des meilleures voitures de la sceance qui viens de s'achever (en fonction de baisse)...
        printf("     %d%s| %d%s|   %.2f\n" ,i+1,(i < 9) ? "    " : "   ", classements[i].id, (classements[i].id < 9) ? "    " : "   ", 
        classements[i].meilleursTemps[3]);//et affichage de celles-ci avec leur meilleur temps au tour
    }
}

//Fonction principale du programme
int main (int argc, char *argv[]) {
    int key = 50; //Cle d'acces a la memoire partagee
    struct Voiture *refVoitures;//Reference pour la quantite de memoire necessaire a l'alocation de la memoire partagee

    int sem_set_id;//Variable pour contenir l'id d'acces aux semaphores
    union semun sem_val;//Variable pour contenir la valeur de la semaphore
    int SEM_ID = 250;//Cle d'acces aux semaphores
    
    if ((sem_set_id = semget(SEM_ID, 1, IPC_CREAT | 0600)) == -1) {//Creation d'un set de semaphores avec la cle SEM_ID, ne contient qu'une semaphore...
	    perror("main: semget");//Traitement en cas d'erreur
	    exit(1);
    }
    sem_val.val = 1;//Attribution d'une valeur a sem_val
    if (semctl(sem_set_id, 0, SETVAL, sem_val) == -1) {//initialisation de la semaphore a 1 et...
	    perror("main: semctl");//traitement en cas d'erreur
	    exit(1);
    }

    int num;//Variable recuperant la valeur entree par l'utilisateur
    printf("Choisissez la taile du circuit (en kilometres)\n");//Demande a l'utilisateur d'entrer un nombre representant la taille en km du circuit
    while (scanf("%d",&num) != 1) {//Verifie la valeur entree par l'utilisateur : tant qu'il ne s'agit pas d'un nombre...
        getchar();//supprime le caractere errone entre par l'utilisateur...
        printf ("Entrez bien un nombre\n");//redemande a l'utilisateur d'entrer un nombre
    }
    tempsSecteur = 4*num;//Attribution d'une valeur, proportionnelle au nombre entre par l'utilisateur, a la variable generale tempsSecteur
    
    int voituresQ1[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};//Tableau comprenant les numeros de toutes le voitures inscrites au grand prix
    int nombreVoiture = sizeof(voituresQ1) / sizeof(voituresQ1[0]);//Calcul du nombre de voiture...
    //calcul du nombre d'elements de voituresQ1 en divisant la taille (en bits) totale de voituresQ1 par la taille (en bits) de son premier element
    
    //------------------------------------------P1----------------------------------------------------------
    trigger = 9;//Attribution d'une valeur, proportionnelle a la duree de la sceance sur le point d'etre lancee, a la variable generale trigger
    wait("P1");//Appel de la methode wait() sur F1.c
    qualif(voituresQ1,nombreVoiture,key,0, sem_set_id);//Appel de la methode qualif() sur F1.c
    key+=5;//Incrementation de key afin de la reutiliser pour creer de nouvelles memoires partagees

    //------------------------------------------P2----------------------------------------------------------
    trigger = 9;//Attribution d'une valeur a trigger
    wait("P2");//Appel de la methode wait() sur F1.c
    qualif(voituresQ1,nombreVoiture,key,0, sem_set_id);//Appel de la methode qualif() sur F1.c
    key+=5;//Incrementation de key

    //------------------------------------------P3----------------------------------------------------------
    trigger = 6;//Attribution d'une valeur a trigger
    wait("P3");//Appel de la methode wait() sur F1.c
    qualif(voituresQ1,nombreVoiture,key,0, sem_set_id);//Appel de la methode qualif() sur F1.c
    key+=5;//Incrementation de key

    //------------------------------------------Q1----------------------------------------------------------
    trigger = 6;//Attribution d'une valeur a trigger
    wait("Q1");//Appel de la methode wait() sur F1.c
    qualif(voituresQ1,nombreVoiture,key,5, sem_set_id);//Appel de la methode qualif() sur F1.c
    key+=5;//Incrementation de key

    //------------------------------------------Q2----------------------------------------------------------
    trigger = 5;//Attribution d'une valeur a trigger
    nombreVoiture -= 5;//Ajustement du nombre de voitures au depart de Q2
    int voituresQ2[nombreVoiture];//Creation d'un nouveau tableau qui contiendra les numeros des meilleures voitures de Q1
    for (int i=0;i<nombreVoiture;i++) {//Boucle qui ira recuperer les 15 premiers ids du classement...
        voituresQ2[i] = classements[i].id;//c'est a dire les numeros des 15 voitures selectionnees pour participer a Q2
    }
    wait("Q2");//Appel de la methode wait() sur F1.c
    qualif(voituresQ2,nombreVoiture,key,5, sem_set_id);//Appel de la methode qualif() sur F1.c
    key+=5;//Incrementation de key

    //------------------------------------------Q3----------------------------------------------------------
    trigger = 4;//Attribution d'une valeur a trigger
    nombreVoiture -= 5;//ajustement du nombre de voitures au depart de Q3
    int voituresQ3[nombreVoiture];//Creation d'un nouveau tableau qui contiendra les numeros des meilleures voitures de Q2
    for (int i=0;i<nombreVoiture;i++) {//Boucle qui ira recuperer les 10 premiers ids du classement...
        voituresQ3[i] = classements[i].id;//c'est a dire les numeros des 10 voitures selectionnees pour participer a Q3
    }
    wait("Q3");//Appel de la methode wait() sur F1.c
    qualif(voituresQ3,nombreVoiture,key,0, sem_set_id);//Appel de la methode qualif() sur F1.c

    //----------------------------------------Classement Qualifs---------------------------------------------------------
    nombreVoiture = sizeof(voituresQ1) / sizeof(voituresQ1[0]);//Retablissement du nombre de voitures au depart de la course
    int course[nombreVoiture];//Creation d'un nouveau tableau qui contiendra les numeros des voitures au depart de la course, dans l'ordre du classement a l'issue des qualifications
    printf("\n  Grille de depart de la course :\n");//Annonce que l'affichage suivant represente la grille de depart de la course
    printf("\n    Place | n°   | Meilleur temps\n");//Ecriture des noms des colonnes de l'affichage
    for (int i=0;i<nombreVoiture;i++) {//Recuperation de l'ordre du classement des voitures apres les qualifications...
        course[i] = classements[i].id;//memorisation des numeros de voitures et de leur ordre dans course...
        printf("     %d%s| %d%s|   %.2f\n" ,i+1,(i < 9) ? "    " : "   ", course[i], (course[i] < 9) ? "    " : "   ", classements[i].meilleursTemps[3]);
        //et affichage de la grille de depart et des meilleurs temps au tour respectifs des voitures lors de leur derniere sceance de qualifications
    }

    //----------------------------------------Course---------------------------------------------------------
    int shmid;//Variable pour contenir l'id d'acces a la memoire partagee
    if ((shmid = shmget(3, nombreVoiture*sizeof(refVoitures) , IPC_CREAT | 0666 )) == -1) {//initialisation de la memoire partagee a l'aide d'une cle de valeur 3, memorisation de l'id d'acces dans shmid et...
        printf("Erreur : shmget\n");//Traitement en cas d'erreur
        return -1;
    };

    int newNum;//Variable recuperant la valeur entree par l'utilisateur
    printf("Entrez un nombre pour passer a la course");//Demande a l'utilisateur d'entrer un nombre afin de continuer l'execution du programme
    while (scanf("%d",&newNum) != 1) {//Verifie la valeur entree par l'utilisateur : tant qu'il ne s'agit pas d'un nombre...
        getchar();//supprime le caractere errone entre par l'utilisateur...
        printf ("Entrez bien un nombre !\n");//redemande a l'utilisateur d'entrer un nombre
    }

    forkCourse(course, shmid, nombreVoiture, sem_set_id);//Appel de la methode forkCourse() sur fork.c

    //----------------------------------------Affichage des resultats de la course-----------------------------
    printf("\n\n  Resultats de la course :\n");//Annonce que l'affichage suivant represente les resultats de la course
    printf("\n    Place | n°   | Meilleur temps | Temps total  |\n");//Ecriture des noms des colonnes de l'affichage
    for (int i=0;i<nombreVoiture;i++) {//Recuperation de l'ordre du classement des voitures apres les qualifications...
        printf("     %d%s| %d%s|     %.2f     |    %.2f   %s|\n" ,i+1,(i < 9) ? "    " : "   ", classements[i].id,
        (classements[i].id < 9) ? "    " : "   ", classements[i].meilleursTemps[3], 
        classements[i].tempsTotalCourse, ((classements[i].tempsTotalCourse < 999.99) ? " " : ""));
        //et affichage des resultats (meilleurs temps au tour et temps total respectifs des voitures lors de la course)
    }//recuperation du classement final dans voitures et affichage du nombre de tours, du temps total et du meilleur temps au tour de chaque voiture
    printf("\n\n\n\n");//retour a la ligne multiple pour raisons esthetiques uniquement

    return 0;
}
