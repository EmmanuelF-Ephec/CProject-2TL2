#include <sys/types.h>
#include <unistd.h>

void forkVoitures(int tab[20]) {
    int pid_fils;
    int pere = getpid();
    printf("pere %d\n" , pere);
    for (int i = 0; i < 20; i++) {
    
        pid_fils = fork();
        if (pid_fils == 0) {
            printf("Je suis le fils %d\n" , getpid());
            exit(0);
        }
        else
        {
            printf("je suis le pere\n");
        }
        
    }
    return 0;
}
