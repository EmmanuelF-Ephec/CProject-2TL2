#define SHM_SIZE 1024

int shmid;
struct Voiture *getVoitures;

int getId;
double temps;


int memPart () {
    if ((shmid = shmget(77, SHM_SIZE , 0777 | IPC_CREAT )) == -1) {
        printf("Erreur : shmget\n");
        return -1;
    };
    if ((getVoitures = shmat(shmid, 0, 0) == -1)) {
        printf("Erreur : shmat\n");
        return -1;
    }
    printf("L'id de la voiture est %d\n", getVoitures);

    return 0;
}

