//Fonction permettant de creer un processus fils pour chaque voiture participant a la sceance en cours et d'attribuer un role different aux fils et au pere
//int tab est le tableau contenant les numeros des voitures participant a la sceance en cours
//int shmid contient l'id d'acces a la memoire partagee de la sceance en cours
//size_t nombreVoiture correspond au nombre d'elements contenus dans tab (nombre de voitures au depart de la sceance)
//int sem_set_id contient l'id d'acces aux semaphores
void forkVoitures(int tab[20], int shmid, size_t nombreVoiture, int sem_set_id) {
    int pid_fils;//Variable qui enregistrera le return de fork, et permet donc l'identification des processus pere et fils
    int pere = getpid();//Recuperation de l'id du processus en cours d'execution (le processus pere)

    struct Voiture *getVoitures;//Pointeur qui contiendra l'adresse d'acces a la memoire partagee associee a la sceance en cours
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {//Recuperation de l'adresse d'acces a la memoire partagee et...
        printf("Erreur : shmat\n");//traitement en cas d'erreur
    }

    for (int numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {//Cette boucle va parcourir la memoire partagee nouvellement cree et...
        getVoitures[numVoiture].id = tab[numVoiture];//Initialiser les numeros des voitures en lisse pour la sceance en cours
    }

    for (int numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {//Cette boucle va se repeter autant de fois que de voitures en lisse pour la sceance en cours et...
        if ((pid_fils = fork()) == -1) {//crer autant de processus fils que necessaire (un pour chaque voiture) et...
            printf("Erreur : fork");//traitement en cas d'erreur
        };

        if (pid_fils == 0) {//Si le processus en cours d'execution correspond a une voiture (processus fils)
            int id = getpid();//Recuperation de l'id du processus en cours d'execution
            rouler(shmid,numVoiture,id, sem_set_id);//Appel de la methode rouler sur randGen.c
            exit(0);//Arret d'execution du processus fils
        }
    }
    if (pid_fils > 0) {//Si le processus en cours d'execution correspond a l'afficheur (processus pere)
        struct Voiture c;//Variable intermediaire pour le tri du classement
        struct Voiture classement[nombreVoiture];//Classement des voitures
        double meilleursTemps[nombreSecteurs];//Variable qui recuperera le meilleur temps pour chaque secteur
        int meilleursId[nombreSecteurs];//Variable qui recuperera le numero de la voiture ayant effectue le meilleur temps pour chaque secteur
        char *secteur;//Variable qui permettra de marquer par une balise les voitures dont le numero est enregistre dans meilleursId

        for (compteur = 0;compteur<trigger;compteur++) {//Boucle destinee a afficher un nombre fixe de fois les resultats avant de cloturer la sceance en cours
            memcpy(classement, getVoitures, nombreVoiture*sizeof(struct Voiture));//Copie de la memoire partagee dans le classement (en variable locale)
            sleep(3);//Le processus en cours d'execution marque une petite pause avant de reprendre l'execution

            for(int i=0;i<nombreVoiture;i++) {//Boucle destinee a parcourir tous les elements de classement
                for(int j=i+1;j<nombreVoiture;j++) {//Boucle destinee a parcourir tous les elements de classement suivants celui selectionne par la boucle ci-dessus
                    if ( classement[i].meilleursTemps[3] > classement[j].meilleursTemps[3] && classement[j].meilleursTemps[3] != 0) {//si l'element selectionne par j a un meilleur temps au tour que l'element selectionne par i...
                        c = classement[i];//memorisation temporaire de i (hors de classement)
                        classement[i] = classement[j];//memorisation de j a la place de i (ecrasement des informations contenues dans i)
                        classement[j] = c;//memorisation de c (memorisation temporaire de i) a la place de j (ecrasement des informations contenues dans j)
                    }
                }
            }
            for(int i=0;i<nombreVoiture;i++) {//Boucle destinee a parcourir tous les elements de classement
                for (int k=0;k<nombreSecteurs;k++) {//Boucle destinee a parcourir tous les temps secteurs memorises dans chaque element de classement
                    if (classement[i].meilleursTemps[k] < meilleursTemps[k] || meilleursTemps[k] == 0) {//Si le temps secteur de i est meilleur (plus petit) que le temps enregistre dans meilleursTemps...
                        meilleursTemps[k] = classement[i].meilleursTemps[k];//Enregistrement du temps secteur de i a la place du temps enregistre dans meilleursTemps
                        meilleursId[k] = classement[i].id;//Enregistrement du numero de la voiture i a la place du numero enregistre dans meilleursId
                    }
                }
            }

            printf("\n   Voiture   |  Temps S1     |    Temps S2   |    Temps S3   |   Temps tour   | Nombre passages au stand|   IN/OUT   |  Meilleur secteur   \n");//Ecriture des noms des colonnes de l'affichage
            for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {//Boucle destinee a parcourir tous les elements de classement
                secteur = "";//Initialisation de la variable secteur
                for (int i=0;i<nombreSecteurs;i++) {//Boucle destinee a parcourir tous les elements de meilleursId
                    if (classement[cptVoiture].id == meilleursId[i]) {//Si le numero de la voiture i correspond au numero enregistre dans meilleursId...
                        asprintf(&secteur,"%s S%d",secteur,i+1);//la ligne des informations associees a la voiture i sera marquee par la balise appropriee
                    }
                }
                if (classement[cptVoiture].estOut == 0){
                    printf("     n°%d %s|     %.2f %s|     %.2f %s|     %.2f %s|     %.2f %s|           %d %s|     IN     |    %s \n",       //affichage des voitures encore dans la course
                    classement[cptVoiture].id, ((classement[cptVoiture].id < 10) ? "    " : "   "),
                    classement[cptVoiture].meilleursTemps[0], ((classement[cptVoiture].meilleursTemps[0] < 10) ? "     " : "    "), 
                    classement[cptVoiture].meilleursTemps[1], ((classement[cptVoiture].meilleursTemps[1] < 10) ? "     " : "    "),             //affichage dynamique pour que les colonnes soient toujorus alignées
                    classement[cptVoiture].meilleursTemps[2],((classement[cptVoiture].meilleursTemps[2] < 10) ? "     " : "    "),      
                    classement[cptVoiture].meilleursTemps[3], ((classement[cptVoiture].meilleursTemps[3] < 10) ? "     " : "    "),
                    classement[cptVoiture].auStand, ((classement[cptVoiture].auStand < 10) ? "            " : "           "),
                    secteur);
                    }//et affichage du meilleurs temps de chaque secteur et du meilleur temps tour pour chaque voiture participant
                }
            for (int cptVoiture = 0; cptVoiture < nombreVoiture; cptVoiture ++){
                 if (classement[cptVoiture].estOut == 1){
                    printf("     n°%d %s|     %.2f %s|     %.2f %s|     %.2f %s|     %.2f %s|           %d %s|    OUT     |    %s\n",             //affichage des voitures ecrasées
                    classement[cptVoiture].id, ((classement[cptVoiture].id < 10) ? "    " : "   "),
                    classement[cptVoiture].meilleursTemps[0], ((classement[cptVoiture].meilleursTemps[0] < 10) ? "     " : "    "),
                    classement[cptVoiture].meilleursTemps[1], ((classement[cptVoiture].meilleursTemps[1] < 10) ? "     " : "    "),             //affichage dynamique pour que les colonnes soient tjr alignées
                    classement[cptVoiture].meilleursTemps[2],((classement[cptVoiture].meilleursTemps[2] < 10) ? "     " : "    "),
                    classement[cptVoiture].meilleursTemps[3], ((classement[cptVoiture].meilleursTemps[3] < 10) ? "     " : "    "),
                    classement[cptVoiture].auStand, ((classement[cptVoiture].auStand < 10) ? "            " : "           "),
                    secteur);
                 }
            }
        }
        memcpy(classements,classement,sizeof(classement));//Copie du classement (en variable locale) en variable generale
    }
}

//Fonction tres similaire a forkCourse
//int tab est le tableau contenant les numeros des voitures participant a la sceance en cours
//int shmid contient l'id d'acces a la memoire partagee de la sceance en cours
//size_t nombreVoiture correspond au nombre d'elements contenus dans tab (nombre de voitures au depart de la sceance)
//int sem_set_id contient l'id d'acces aux semaphores
void forkCourse(int tab[20], int shmid, size_t nombreVoiture, int sem_set_id){
    int pid_fils;//Identification entre processus pere et fils
    int pere = getpid();//Recuperation de l'id du processus en cours d'execution (le processus pere)

    struct Voiture *getVoitures;//Pointeur qui contiendra l'adresse d'acces a la memoire partagee associee a la sceance en cours
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {//Recuperation de l'adresse d'acces a la memoire partagee et...
        printf("Erreur : shmat\n");//traitement en cas d'erreur
    }

    for (int numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {//Cette boucle va parcourir la memoire partagee nouvellement cree et...
        getVoitures[numVoiture].id = tab[numVoiture];//Initialiser les numeros des voitures en lisse pour la sceance en cours
    }

    for (int numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {//Cette boucle va se repeter autant de fois que de voitures en lisse pour la sceance en cours et...
        if ((pid_fils = fork()) == -1) {//crer autant de processus fils que necessaire (un pour chaque voiture) et...
            printf("Erreur : fork");//traitement en cas d'erreur
        };

        if (pid_fils == 0) {//Si je suis une voiture
            int id = getpid();//Recuperation de l'id du processus en cours d'execution
            roulerCourse(shmid, numVoiture, id, sem_set_id);//Appel de la methode roulerCourse sur randGen.c
             if (getVoitures[numVoiture].auStand < 1 && getVoitures[numVoiture].estOut == 0) {//Si la voiture (processus fils) ne s'est pas arrete aux stands et ne s'est pas crashee a l'issue de la course...
                getVoitures[numVoiture].tempsTotalCourse += (genRandomNbr(199,399) / 100.00);//on force un passage au stand
            }
            getVoitures[numVoiture].validation = 1;//attribution de la valeur 1 au champ validation de la voiture (elle a fini de parcourir le nombre de tours precise par la variable nombreToursCourse definie dans struct.h)
            exit(0);//Arret d'execution du processus fils
        }
    }
    if (pid_fils > 0) {//Si je suis le pere (l'afficheur)...
        struct Voiture c;//Variable intermediaire pour le tri du classement
        struct Voiture classement[nombreVoiture];//Classement des voitures
        int validation = 0;//variable de comptage
        double meilleursTemps[nombreSecteurs];//Variable qui recuperera le meilleur temps pour chaque secteur
        int meilleursId[nombreSecteurs];//Variable qui recuperera le numero de la voiture ayant effectue le meilleur temps pour chaque secteur
        char *secteur;//Variable qui permettra de marquer par une balise les voitures dont le numero est enregistre dans meilleursId
        
        while (validation != nombreVoiture) {//Tant que validation n'est pas egalle au nombre de voitures participant a la course (c'est a dire tant que chaque voiture n'a pas fait le nombre de tours requis)...
            validation = 0;//Initialisation/reinitialisation de validation
            if (validation != nombreVoiture) {
                memcpy(classement, getVoitures, nombreVoiture*sizeof(struct Voiture));//Copie de la memoire partagee dans le classement (en variable locale)
                sleep(2);//Le processus en cours d'execution marque une petite pause avant de reprendre l'execution
                for (int i = 0;i<nombreVoiture;i++) {//Boucle destinee a parcourir chaque element de classement
                    if (classement[i].validation == 1) {//Si le champ validation de la voiture i est a 1...
                        validation++;//Incrementation de validation
                    }
                }
                for(int i=0;i<nombreVoiture;i++) {//Boucle destinee a parcourir tous les elements de classement
                    for(int j=i+1;j<nombreVoiture;j++) {//Boucle destinee a parcourir tous les elements de classement suivants celui selectionne par la boucle ci-dessus
                        if ( classement[i].nombreTour < classement[j].nombreTour) {//si l'element selectionne par j a fais plus de toursque l'element selectionne par i...
                            c = classement[i];//memorisation temporaire de i (hors de classement)
                            classement[i] = classement[j];//memorisation de j a la place de i (ecrasement des informations contenues dans i)
                            classement[j] = c;//memorisation de c (memorisation temporaire de i) a la place de j (ecrasement des informations contenues dans j)
                        }
                        else if ( classement[i].nombreTour== classement[j].nombreTour){//Si l'element selectionne par j a fait autant de tours que celui selectionne par i...
                            if ( classement[i].tempsTotalCourse > classement[j].tempsTotalCourse) {//Et si l'element selectionne par j a un meilleur temps total (plus petit) que l'element selectionne par i...
                                c = classement[i];//memorisation temporaire de i (hors de classement)
                                classement[i] = classement[j];//memorisation de j a la place de i (ecrasement des informations contenues dans i)
                                classement[j] = c;//memorisation de c (memorisation temporaire de i) a la place de j (ecrasement des informations contenues dans j)
                            }
                        }
                    }
                }
                for(int i=0;i<nombreVoiture;i++) {//Boucle destinee a parcourir tous les elements de classement
                    for (int k=0;k<nombreSecteurs;k++) {//Boucle destinee a parcourir tous les temps secteurs memorises dans chaque element de classement
                        if (classement[i].meilleursTemps[k] < meilleursTemps[k] || meilleursTemps[k] == 0) {//Si le temps secteur de i est meilleur (plus petit) que le temps enregistre dans meilleursTemps...
                            meilleursTemps[k] = classement[i].meilleursTemps[k];//Enregistrement du temps secteur de i a la place du temps enregistre dans meilleursTemps
                            meilleursId[k] = classement[i].id;//Enregistrement du numero de la voiture i a la place du numero enregistre dans meilleursId
                        }
                    }
                }
                printf("\n   Voiture   |    Temps S1    |    Temps S2    |    Temps S3    |   Temps tour    |  Temps total   |    Tours   | Nombre de passages au stand |    IN/OUT   |Meilleur Secteur  \n");//Ecriture des noms des colonnes de l'affichage
                for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {//Boucle destinee a parcourir tous les elements de classement
                    secteur = "";//Initialisation de la variable secteur
                    for (int i=0;i<nombreSecteurs;i++) {//Boucle destinee a parcourir tous les elements de meilleursId
                        if (classement[cptVoiture].id == meilleursId[i]) {//Si le numero de la voiture i correspond au numero enregistre dans meilleursId...
                            asprintf(&secteur,"%s S%d",secteur,i+1);//la ligne des informations associees a la voiture i sera marquee par la balise appropriee
                        }
                    }
                    printf("     n°%d %s|     %.2f %s|     %.2f %s|     %.2f %s|     %.2f %s|     %.2f %s|     %d %s|              %d %s|      %s    |    %s   \n",     //Affichage de chaque voiture et de ses temps
                    classement[cptVoiture].id, ((classement[cptVoiture].id < 10) ? "    " : "   "), 
                    classement[cptVoiture].meilleursTemps[0], ((classement[cptVoiture].meilleursTemps[0]) ? "     " : "    "), 
                    classement[cptVoiture].meilleursTemps[1], ((classement[cptVoiture].meilleursTemps[1]) ? "     " : "    "),                       //Affichage dynamique pour que les colonnes soient tjr au meme niveau
                    classement[cptVoiture].meilleursTemps[2], ((classement[cptVoiture].meilleursTemps[2]) ? "     " : "    "), 
                    classement[cptVoiture].meilleursTemps[3], ((classement[cptVoiture].meilleursTemps[3]) ? "     " : "    "),              
                    classement[cptVoiture].tempsTotalCourse, ((classement[cptVoiture].tempsTotalCourse < 999.99) ? "    " : "   "), 
                    classement[cptVoiture].nombreTour, ((classement[cptVoiture].nombreTour < 10) ? "     " : "    "),
                    classement[cptVoiture].auStand, ((classement[cptVoiture].auStand < 10) ? "             " : "           "),
                    ((classement[cptVoiture].estOut == 0) ? "IN " : "OUT"),
                    secteur);
                    //et affichage du meilleurs temps de chaque secteur ainsi du meilleur temps tour, du temps total et du nombre de tours pour chaque voiture participant a la course
                }
            }
        }
        memcpy(classements, classement, sizeof(classements));//Copie du classement (en variable locale) en variable generale
    }
}
