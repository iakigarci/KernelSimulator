#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>    // srand, rand,...
#include <time.h>      // time 

#define MAXTHREAD       8
#define DELAY_TIMER     5
#define BUFFER_MAX      5
#define WAITING_TO_EXIT 30

typedef struct { 
  int i; 
  pthread_t tid; 
} identif_t; 

struct parametros {
	int tid;
	int frec;
};

struct PCB {
	int id;
}; 

typedef struct {
   struct PCB dat[BUFFER_MAX];
   int r;
   int w;
   int size;
} buffer_d;

buffer_d buffer;
pthread_mutex_t mutex, mutexC;
sem_t sem_cola;
int clockTime;

void *kernelClock(void *arg); 
void *timer(void *arg); 
void *processGenerator(void *arg); 
void *scheduler(void *arg); 

void inicializar();
int meter_elem(struct PCB pcb);
int sacar_elem(struct PCB *pcb);
int mensaje_error(char *s);

/*----------------------------------------------------------------- 
 *   main
 *----------------------------------------------------------------*/

/*
	1	clockFrec
	2	timerFrec
	3	processFrec
*/
int main(int argc, char *argv[]) {
	identif_t idtimer, idclock, idscheduler, idprocessgenerator;

	if (argc != 4)
	{
		mensaje_error("Los parametros sos: ./scritp frecuenciaClock frecuenciaTimer frecuenciaProcessGen");
	}
	struct parametros p1; //	clock
	struct parametros p2; //	timer
	struct parametros p3; //	processGenerator
	struct parametros p4; //	scheduler

	inicializar();

	p1.tid=idclock.tid;
	p1.frec=atoi(argv[1]);
	p2.tid=idtimer.tid;
	p2.frec=atoi(argv[2]);
	p3.tid=idprocessgenerator.tid;
	p3.frec=atoi(argv[3]);
	p4.tid=idscheduler.tid;
	printf("Comienza el programa\n");
	//pthread_create(&(idHilo),atributosHilo,funcion, parametroVoid)
	pthread_create(&(idclock.tid),NULL,kernelClock,(void *)&p1);
	pthread_create(&(idtimer.tid),NULL,timer,(void *)&p2);
	pthread_create(&(idprocessgenerator.tid),NULL,processGenerator,(void *)&p3);
	pthread_create(&(idscheduler.tid),NULL,scheduler,(void *)&p4);

	sleep(WAITING_TO_EXIT);
	printf("Fin\n");
    return(0);
}

/*----------------------------------------------------------------- 
 *   inicializar
 *----------------------------------------------------------------*/

void inicializar() {
   pthread_mutex_init(&mutex, NULL);
   pthread_mutex_init(&mutexC, NULL);
   sem_init(&sem_cola, 0, 0);

   clockTime=0;
   /* buffer */
   int i;
   buffer.r = 0;
   buffer.w = 0;
   buffer.size = 0;
   struct PCB pcb;
   for(i = 0; i < BUFFER_MAX; i++) buffer.dat[i] = pcb;
} // inicializar

/*----------------------------------------------------------------- 
 *   clock
 *----------------------------------------------------------------*/

void *kernelClock(void *arg) {
	struct parametros *p;
	p = (struct parametros *)arg;
	int count;
	while(1) {
		count++;
		// Aproximado: 1s
		if (count == 100000000)
		{
			count=0;
			pthread_mutex_lock(&mutexC);
			clockTime++;
			printf("  CLOCK[%d] \n", clockTime);
			pthread_mutex_unlock(&mutexC);	
		}
	}
}
/*----------------------------------------------------------------- 
 *   timer
 *----------------------------------------------------------------*/

void *timer(void *arg) {
	struct parametros *p;
	p = (struct parametros *)arg;
	int id = p -> tid;
	int frec = p -> frec;
	while(1) {
		pthread_mutex_lock(&mutexC);
		if (clockTime>=frec)
		{
			clockTime=0;
			printf("  TIMER[%d] avisa\n", id);
		}
		pthread_mutex_unlock(&mutexC);	
	}
} 
/*----------------------------------------------------------------- 
 *   process generator
 *----------------------------------------------------------------*/

void *processGenerator(void *arg) {
	struct parametros *p;
	p = (struct parametros *)arg;
	int id = p -> tid;
	int frec = p -> frec;
	int i=0;
	struct PCB pcb;
	while(1) {
		sleep(frec);
		pcb.id=i;
		printf("1\n");
		//sem_wait(&sem_cola);
		printf("2\n");
		pthread_mutex_lock(&mutex);
		printf("3\n");
		if (meter_elem(pcb)) {
            printf("  Process Generator[%d] produce %02d\n", id, pcb.id);
        }
        else {
            mensaje_error("Buffer lleno");
        }
		pthread_mutex_unlock(&mutex);
		//sem_post(&sem_cola);
		i++;	
	}
}

/*----------------------------------------------------------------- 
 *   process generator
 *----------------------------------------------------------------*/
void *scheduler(void *arg) {
	struct parametros *p;
	p = (struct parametros *)arg;
	int id = p -> tid;
	printf("  Soy un Scheduler con número [%d] \n", id);
}
/*----------------------------------------------------------------- 
 *   meter un elemento al buffer
 *----------------------------------------------------------------*/
int meter_elem(struct PCB pcb) {

   if(buffer.size < BUFFER_MAX) {
      buffer.dat[buffer.w] = pcb;
      buffer.w = (buffer.w + 1) % BUFFER_MAX;
      buffer.size++;
      return 1;
   }
   else { /* buffer lleno */
      return 0;
   }
} 

/*----------------------------------------------------------------- 
 *   sacar un elemento del buffer
 *----------------------------------------------------------------*/

int sacar_elem(struct PCB *pcb) {
   /* When the buffer is not huecos remove the pcb
      and decrement the counter */
   if(buffer.size > 0) {
      *pcb = buffer.dat[buffer.r];
      buffer.r = (buffer.r + 1) % BUFFER_MAX;
      buffer.size--;
      return 1;
   }
   else { /* buffer vacío */
      return 0;
   }
}

/*----------------------------------------------------------------- 
 *   mensajes
 *----------------------------------------------------------------*/

int mensaje_error(char *s) {
    printf("***error*** %s\n",s);
    exit(-1);
}