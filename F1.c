#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int timer() {
    srand(time(NULL));
    int time1 = rand() % 15 + 35;
    //printf("temps : %d \n", time1); 
    return time1;
}

int main (int argc, char *argv[]) {

    int pid_fils;
    int voitures[20] = { 7, 99, 5, 16, 8, 20, 4, 55, 10, 26, 44, 77, 11, 18, 23, 33, 3, 27, 63, 88};

    int pere = getpid();
    printf("pere %d\n" , pere);
    for (int i = 0; i < 4; i++)
    {
    
        pid_fils = fork();
        if (pid_fils == 0) {
            int num = voitures[i];
            printf("La voiture avec le numéro %d a fait la section 1 avec le temps %d \n",num, timer());
            //printf("Le fils avec pid %d et num %d et pere a pid %d\n",getpid(),num, getppid()); 
            exit(0);
        }
        else
        {
            printf("je suis le pere\n");
        }
        
    }

    return 0;
}