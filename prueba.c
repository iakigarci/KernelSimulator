#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>    // srand, rand,...
#include <time.h>      // time 
#include <stdbool.h>
#include <limits.h> 

typedef struct s {
    int a;
}s;
int prueba();

int main(int argc, char *argv[]) {
    s* array_s[3];

    struct s sa;
    sa.a=0;
    struct s sb;
    sb.a=22;
    array_s[1] = &sa; // Meto puntero

    printf("%d\n",array_s[1]->a);
    array_s[1] = &sb;
    printf("%d\n",array_s[1]->a);
 }
