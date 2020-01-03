struct Voiture {
    int id;
    //double S1, S2, S3, tempsTotal;
    double meilleursTemps[4];
    int estOut;
    int auStand;
    int nombreTour;
    double tempsTotalCourse;
    int validation;
};

int compteur;
int trigger = 4;
int nombreToursCourse = 10;
int nombreSecteurs = 3;

struct Voiture classements[20];

union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
};