struct Voiture {//Creation du squelette d'une structure Voiture. Cela nous permettra d'employer des memoires partagees en gardant une organisation identique partout
    int id;//Correspond au numero de la voiture
    double meilleursTemps[4];//Tableau contenant le meilleur temps S1, S2 et S3 ainsi que le meilleur temps tour
    int estOut;//Champ indiquant si une voiture s'est crashee ou non
    int auStand;//Champ indiquant le nombre de passages au stand
    int nombreTour;//Champ indiquant le nombre de tours
    double tempsTotalCourse;//Champ indiquand le temps total ecoule depuis le depart de la course
    int validation;//Champ indiquant si la voiture a termine tous ses tours ou non
};

int compteur;//Variable de comptage déterminant quand doivent s'arrêter les processus lors des essais et qualifications
int trigger;//Variable determinant le nombre de rafraichissements de l'affichage a chaque sceance d'essai ou de qualifications
int nombreToursCourse = 10;//Nombre de tours a effectuer lors de la course
int nombreSecteurs = 3;//Nombre de secteurs composant un tour de piste
int tempsSecteur;//Variable proportionnelle au nombre de km composant un tour de piste (permet de generer des nombres pseudo-aleatoires convaincants)

struct Voiture classements[20];//Variable generale permettant de recuperer le classement des sceances au fur et a mesur du grand prix

union semun {
    int val;//Valeur pour SETVAL
    struct semid_ds *buf;//Buffer pour IPC_STAT et IPC_SET
    unsigned short int *array;//Tableau pour GETALL et SETALL
    struct seminfo *__buf;//Buffer pour IPC_INFO
};