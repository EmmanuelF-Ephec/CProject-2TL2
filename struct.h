struct Voiture {
    int id;
    //double S1, S2, S3, tempsTotal;
    double meilleursTemps[4];
    int estOut;
    int auStand;
};

int compteur;
int trigger = 1;

struct Voiture classements[20];