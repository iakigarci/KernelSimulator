#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>    // srand, rand,...
#include <time.h>      // time 
#include <stdbool.h>
#include <limits.h> 

struct s {
    bool a;
};

int main(int argc, char *argv[]) {
    struct s pr;
    pr.a = true;
    if (!pr.a)
    {
        printf("Comienza el programa\n");
    }else {
        printf("NO Comienza el programa\n");
    }
    
}