//#define SHM_SIZE 20

int shmid;
struct Voiture *getVoitures;

int getId;
double temps;


int memPart () {
    if ((shmid = shmget(77, 20*sizeof(getVoitures) , IPC_CREAT | 0666 )) == -1) {
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

