#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct Voiture {
    int id;
    double S1, S2, S3, tempsTotal;
};