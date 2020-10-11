/*----------------------------------------------------------------------------- 
 *
 *   prodcons.c 
 *
 *   gcc -o prodcons prodcons.c -pthread
 *   ./prodcons 4 2
 *
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>    // srand, rand,...
#include <time.h>      // time 

#define MAXTHREAD       8
#define BUFFER_MAX      5
#define DELAY_PROD      5
#define DELAY_CONS      4
#define WAITING_TO_EXIT 30

typedef struct {
   int dat[BUFFER_MAX];
   int r;
   int w;
   int size;
} buffer_d;

buffer_d buffer;

typedef struct { 
  int i; 
  pthread_t tid; 
} identif_t; 

pthread_mutex_t mutex;
sem_t objetos, huecos;

void *productor(void *tid); 
void *consumidor(void *tid); 

void inicializar();
int meter_elem(int elem);
int sacar_elem(int *elem);
int mensaje_error(char *s);
void buffer_dump();

/*----------------------------------------------------------------- 
 *   main
 *----------------------------------------------------------------*/

int main(int argc, char *argv[]) {
   // Inicializa las estructuras de threads
   identif_t idprod[MAXTHREAD], idcons[MAXTHREAD];
   int tiempo_dormido, product_max, consum_max;
   int i;

   /* Toma y validación de prámetros */

   if(argc != 3) {
      mensaje_error("LLAMADA:./prodcons productores consumidores");
   }

   product_max = atoi(argv[1]); /* Número de productores */
   if((product_max < 0)||(product_max > MAXTHREAD)) {
            mensaje_error("Número de productores fuera de rango");
        }
   consum_max = atoi(argv[2]); /* Número de consumidores */
   if((consum_max < 0)||(consum_max > MAXTHREAD)) {
            mensaje_error("Número de consumidores fuera de rango");
        }

   inicializar();

   /* Crear los hilos */
   for(i = 0; i < product_max; i++) {
      idprod[i].i = i;
      pthread_create(&(idprod[i].tid),NULL,productor,&(idprod[i].i));
    }
   for(i = 0; i < consum_max; i++) {
      idcons[i].i = i;
      pthread_create(&(idcons[i].tid),NULL,consumidor,&(idcons[i].i));
   }

   sleep(WAITING_TO_EXIT);

   printf("Fin\n");
   return(0);
} // main

/*----------------------------------------------------------------- 
 *   inicializar
 *----------------------------------------------------------------*/

void inicializar() {
   int i;
   srand (time(NULL)); 

   pthread_mutex_init(&mutex, NULL);
   sem_init(&objetos, 0, 0);
   sem_init(&huecos, 0, BUFFER_MAX);

   /* buffer */
   buffer.r = 0;
   buffer.w = 0;
   buffer.size = 0;
   for(i = 0; i < BUFFER_MAX; i++) buffer.dat[i] = 0;

   // buffer_dump();
} // inicializar

/*----------------------------------------------------------------- 
 *   hilo productor
 *----------------------------------------------------------------*/

void *productor(void *tid) {
   int elem, id;
   
   id = *(int*)tid;
   while(1) {
      sleep(DELAY_PROD);
      // sleep(rand()%DELAY_PROD);
      elem = (rand()%99)+1;  // Rango {1,..,99}
      sem_wait(&huecos);
      pthread_mutex_lock(&mutex); /* sección crítica */
         if (meter_elem(elem)) {
               printf("  Productor[%d] produce %02d\n", id, elem);
               // buffer_dump();
         }
         else {
            mensaje_error("Buffer lleno");
         }
      pthread_mutex_unlock(&mutex);
      sem_post(&objetos);
   }
} // productor

/*----------------------------------------------------------------- 
 *   hilo consumidor
 *----------------------------------------------------------------*/

void *consumidor(void *tid) {
   int elem, id;

   id = *(int*)tid;
   while(1) {
      sleep(DELAY_CONS);
      // sleep(rand()%DELAY_CONS);
      sem_wait(&objetos);
      pthread_mutex_lock(&mutex); /* sección crítica */
         if (sacar_elem(&elem)){
               printf("    Consumidor[%d] consume %02d\n", id, elem);
               // buffer_dump();
         }
         else {
            mensaje_error("Buffer vacío");
         }
      pthread_mutex_unlock(&mutex);
      sem_post(&huecos);
   }
} // consumidor

/*----------------------------------------------------------------- 
 *   añadir un elemento al buffer
 *----------------------------------------------------------------*/
int meter_elem(int elem) {

   if(buffer.size < BUFFER_MAX) {
      buffer.dat[buffer.w] = elem;
      buffer.w = (buffer.w + 1) % BUFFER_MAX;
      buffer.size++;
      return 1;
   }
   else { /* buffer lleno */
      return 0;
   }
} // meter_elem

/*----------------------------------------------------------------- 
 *   sacar un elemento del buffer
 *----------------------------------------------------------------*/

int sacar_elem(int *elem) {
   /* When the buffer is not huecos remove the elem
      and decrement the counter */
   if(buffer.size > 0) {
      *elem = buffer.dat[buffer.r];
      buffer.r = (buffer.r + 1) % BUFFER_MAX;
      buffer.size--;
      return 1;
   }
   else { /* buffer vacío */
      return 0;
   }
} // sacar_elem

/*----------------------------------------------------------------- 
 *   mensajes
 *----------------------------------------------------------------*/

int mensaje_error(char *s) {
    printf("***error*** %s\n",s);
    exit(-1);
} // mensaje_error

void buffer_dump(){
   int i;

   printf("\n+--+--+--+--+--+\n");
   for(i = 0; i < BUFFER_MAX; i++) 
     	printf("|%02d",buffer.dat[i]);
   printf("| << %d objetos\n",
   		buffer.size);
   printf("%*s\n",((buffer.w*3)+3), " ww");
   printf("%*s\n",((buffer.r*3)+3), " rr");
  printf("+--+--+--+--+--+\n\n");

} // buffer_dump


