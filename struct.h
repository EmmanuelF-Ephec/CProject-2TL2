struct Voiture {
    int id;
    double meilleursTemps[4];
    int estOut;
    int auStand;
    int nombreTour;
    double tempsTotalCourse;
    int validation;
};

int compteur;
int trigger;
/*Essais : 1 --> 10min 
Qualifs : 1 --> 3min*/
int nombreToursCourse = 10;
int nombreSecteurs = 3;
int tempsSecteur;

struct Voiture classements[20];
int shmBest;

union semun {
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short int *array;  /* array for GETALL, SETALL */
    struct seminfo *__buf;      /* buffer for IPC_INFO */
};

/*Keys
Instant : 2
Course : 3
*/