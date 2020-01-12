void forkVoitures(int tab[20], int shmid, size_t nombreVoiture) {
    int pid_fils;//Identification entre processus père et fils
    int pere = getpid();

    int numVoiture, numSecteurs;//Variables de comptage
    struct Voiture *getVoitures;//Adresses d'accès à la mémoire partagée
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
        printf("Erreur : shmat\n");
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        getVoitures[numVoiture].id = tab[numVoiture];//Initialisation en mémoire partagée des numéros de voitures
        /*NB - j'ai retiré l'initialisation de estOut et auStand à zéro car c'est déjà fait dans la fonction clearShm sur F1.c*/
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };//Création d'autant de processus fils que nécessaire(=nombre de voitures) avec traitement en cas d'erreur

        if (pid_fils == 0) {//Si je suis une voiture
            int id = getpid();
            rouler(shmid,numVoiture,id);
            exit(0);
        }
    }
    if (pid_fils > 0) {//Si je suis le père (l'afficheur)...
        int i,j,k;//Variables de comptage
        struct Voiture c;//Variable intermédiaire pour le tri du classement
        struct Voiture classement[nombreVoiture];//Classement des voitures
        double meilleursTemps[nombreSecteurs];
        int meilleursId[nombreSecteurs];
        char *secteur;

        for (compteur = 0;compteur<trigger;compteur++) {//boucle destinée à afficher un nombre fixé de fois les résultats avant de clôturer la scéance de qualification
            memcpy(classement, getVoitures, nombreVoiture*sizeof(struct Voiture));//Copie de la mémoire partagée dans le classement (en variable locale)
            sleep(3);

            for(i=0;i<nombreVoiture;i++) {
                for(j=i+1;j<nombreVoiture;j++) {
                    if ( classement[i].meilleursTemps[3] > classement[j].meilleursTemps[3]) {
                        c = classement[i];
                        classement[i] = classement[j];
                        classement[j] = c;
                    }
                }
            }//Tri du classement
            for(i=0;i<nombreVoiture;i++) {
                for (k=0;k<nombreSecteurs;k++) {
                    if (classement[i].meilleursTemps[k] < meilleursTemps[k] || meilleursTemps[k] == 0) {
                        meilleursTemps[k] = classement[i].meilleursTemps[k];
                        meilleursId[k] = classement[i].id;
                    }
                }
            }//Calcul des meilleurs secteurs

            printf("\n   Voiture   |   Temps S1    |    Temps S2   |    Temps S3   |   Temps tour   |   Meilleurs Secteur   \n");
            for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {
                secteur = "";
                for (i=0;i<nombreSecteurs;i++) {
                    if (classement[cptVoiture].id == meilleursId[i]) {
                        asprintf(&secteur,"%s S%d",secteur,i+1);
                    }
                }
                printf("     n°%d %s|     %.2f     |     %.2f     |     %.2f     |     %.2f     |     %s     \n", 
                classement[cptVoiture].id, ((classement[cptVoiture].id < 10) ? "    " : "   "), classement[cptVoiture].meilleursTemps[0],
                 classement[cptVoiture].meilleursTemps[1], classement[cptVoiture].meilleursTemps[2], classement[cptVoiture].meilleursTemps[3],secteur);
            }//affichage instantané de l'état de la qualification
        }
        memcpy(classements,classement,sizeof(classement));
    }
}

void forkCourse(int tab[20], int shmid, size_t nombreVoiture, int sem_set_id){
    int pid_fils;//Identification entre processus père et fils
    int pere = getpid();

    int numVoiture, numSecteurs;//Variables de comptage

    struct Voiture *getVoitures;//Adresses d'accès à la mémoire partagée pour la création des temps
    if ((getVoitures = shmat(shmid, 0, 0)) == NULL) {
        printf("Erreur : shmat\n");
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        getVoitures[numVoiture].id = tab[numVoiture];//Initialisation en mémoire partagée des numéros de voitures
        /* cfr NB de forkVoiture */
    }

    for (numVoiture = 0; numVoiture < nombreVoiture;numVoiture++) {
        if ((pid_fils = fork()) == -1) {
            printf("Erreur : fork");
        };//Création d'autant de processus fils que nécessaire(=nombre de voitures) avec traitement en cas d'erreur

        if (pid_fils == 0) {//Si je suis une voiture
            int id = getpid();
            //rouler(shmid,numVoiture,id);
            //roulerCourse(shmid, numVoiture, id, sem_set_id,shmidInstant);
            roulerCourse(shmid, numVoiture, id, sem_set_id);
             if (getVoitures[numVoiture].auStand < 1 && getVoitures[numVoiture].estOut == 0) {
                getVoitures[numVoiture].tempsTotalCourse += (genRandomNbr(199,399) / 100.00);
                printf("Forcer pit");
            }
            getVoitures[numVoiture].validation = 1;
            exit(0);
        }
    }
    if (pid_fils > 0) {//Si je suis le père (l'afficheur)...
        int i,j,k;//Variables de comptage
        struct Voiture c;//Variable intermédiaire pour le tri du classement
        struct Voiture classement[nombreVoiture];//Classement des voitures
        int tour = 0;//variable de comptage
        double meilleursTemps[nombreSecteurs];
        int meilleursId[nombreSecteurs];
        char *secteur;
        while (tour != nombreVoiture) {
            tour = 0;
            if (tour != nombreVoiture) {
                memcpy(classement, getVoitures, nombreVoiture*sizeof(struct Voiture));//Copie de la mémoire partagée dans le classement (en variable locale)
                sleep(2);
                for (int i = 0;i<nombreVoiture;i++) {
                    if (classement[i].validation == 1) {
                        tour ++;
                    }
                }
                for(int i=0;i<nombreVoiture;i++) {//tri de la shm GID selon ...
                    for(int j=i+1;j<nombreVoiture;j++) {
                        if ( classement[i].nombreTour < classement[j].nombreTour) {//... une majeure (le nombre de tours parcourus) et ...
                            c = classement[i];
                            classement[i] = classement[j];
                            classement[j] = c;
                        }
                        else if ( classement[i].nombreTour== classement[j].nombreTour){
                            if ( classement[i].tempsTotalCourse > classement[j].tempsTotalCourse) {// ... une mineure (le temps nécessaire à parcourir les x tours)
                                c = classement[i];
                                classement[i] = classement[j];
                                classement[j] = c;
                            }
                        }
                    }
                }//Tri du classement
                for(i=0;i<nombreVoiture;i++) {
                    for (k=0;k<nombreSecteurs;k++) {
                        if (classement[i].meilleursTemps[k] < meilleursTemps[k] || meilleursTemps[k] == 0) {
                            meilleursTemps[k] = classement[i].meilleursTemps[k];
                            meilleursId[k] = classement[i].id;
                        }
                    }
                }//Calcul des meilleurs secteurs
                printf("\n   Voiture   |   Temps S1    |    Temps S2   |    Temps S3   |   Temps tour   |   Temps total   |   Tours    |   Meilleur Secteur    \n");
                for (int cptVoiture = 0;cptVoiture<nombreVoiture;cptVoiture++) {
                    secteur = "";
                    for (i=0;i<nombreSecteurs;i++) {
                        if (classement[cptVoiture].id == meilleursId[i]) {
                            asprintf(&secteur,"%s S%d",secteur,i+1);
                        }
                    
                    }
                    printf("     n°%d %s|     %.2f     |     %.2f     |     %.2f     |     %.2f     |     %.2f     %s|     %d     %s|     %s     \n", 
                    classement[cptVoiture].id, ((classement[cptVoiture].id < 10) ? "    " : "   "), 
                    classement[cptVoiture].meilleursTemps[0], classement[cptVoiture].meilleursTemps[1], 
                    classement[cptVoiture].meilleursTemps[2], classement[cptVoiture].meilleursTemps[3], 
                    classement[cptVoiture].tempsTotalCourse, 
                    ((classement[cptVoiture].tempsTotalCourse < 999.99) ? " " : ""), 
                    classement[cptVoiture].nombreTour, ((classement[cptVoiture].nombreTour < 10) ? " " : ""),secteur);
                }
            }//affichage instantané de l'état de la course
        }
        memcpy(classements, classement, sizeof(classements));//copie de la shm GID dans le classement (en variable locale)
    }
}
