#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>    // srand, rand,...
#include <time.h>      // time 
#include <stdbool.h>
#include <limits.h> 

#define MAXTHREAD       8
#define DELAY_TIMER     5
#define BUFFER_MAX      5
#define WAITING_TO_EXIT 30
#define NUM_CPU 		1
#define NUM_CORE		2

typedef struct { 
  int i; 
  pthread_t tid; 
} identif_t; 

struct parametros {
	int tid;
	int frec;
};

/** ESTRUCTURAS CPU **/
typedef struct PCB {
	int id;
	int quantum;
	int prioridad; // 0..3
}; 

struct Queue { 
    int front, rear, size; 
    struct PCB arr_pcb[BUFFER_MAX]; 
};  

typedef struct
{
	_Bool is_process;
	struct PCB t_pcb;
} c_thread;

typedef struct
{
   identif_t arr_th[MAXTHREAD];
} core;

struct cpu
{
   core arr_core[NUM_CORE];
};

/** ---------------- **/
struct Queue *queue0,*queue1,*queue2,*queue3;
pthread_mutex_t mutex, mutexC;
sem_t sem_cola;
int clockTime;

void *kernelClock(void *arg); 
void *timer(void *arg); 
void *processGenerator(void *arg); 
void *scheduler(void *arg); 

void inicializar();
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
	
	//	Inicialización de estructuras	
	identif_t idtimer, idclock, idprocessgenerator;
	struct cpu arr_cpu[NUM_CPU];

	//	Control de parametros de entrada
	if (argc != 4)
	{
		mensaje_error("Los parametros sos: ./scritp frecuenciaClock frecuenciaTimer frecuenciaProcessGen");
	}
	struct parametros p1; //	clock
	struct parametros p2; //	timer
	struct parametros p3; //	processGenerator
	struct parametros p4; //	scheduler

	inicializar();

	//	Inicializacion de los parámetros que van a tener los hilos
	p1.tid=idclock.tid;
	p1.frec=atoi(argv[1]);
	p2.tid=idtimer.tid;
	p2.frec=atoi(argv[2]);
	p3.tid=idprocessgenerator.tid;
	p3.frec=atoi(argv[3]);

	printf("Comienza el programa\n");

	//	Se crean los hilos necesarios para todo el sistema
	//pthread_create(&(idHilo),atributosHilo,funcion, parametroVoid)
	pthread_create(&(idclock.tid),NULL,kernelClock,(void *)&p1);
	pthread_create(&(idtimer.tid),NULL,timer,(void *)&p2);
	pthread_create(&(idprocessgenerator.tid),NULL,processGenerator,(void *)&p3);

	inicializar();
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
   struct Queue* queue0 = createQueue(10);
   struct Queue* queue1 = createQueue(10);
   struct Queue* queue2 = createQueue(10);
   struct Queue* queue3 = createQueue(10);
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
			identif_t idscheduler;
			clockTime=0;
			printf("  TIMER[%d] avisa\n", id);
			pthread_create(&(idscheduler.tid),NULL,scheduler,NULL);
			pthread_exit()
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
		pcb.prioridad = rand() % (3+1-0) + 0;
		//sem_wait(&sem_cola);
		pthread_mutex_lock(&mutex);
		switch (pcb.prioridad)
		{
		case 0:
			enqueue(queue0,pcb);
			break;
		case 1:
			enqueue(queue1,pcb);
			break;
		case 2:
			enqueue(queue2,pcb);
			break;
		case 3:
			enqueue(queue3,pcb);
			break;
		default:
			break;
		} 
        printf("  Process Generator[%d] produce %02d\n", id, pcb.id);
		pthread_mutex_unlock(&mutex);
		//sem_post(&sem_cola);
		i++;	
	}
}

/*----------------------------------------------------------------- 
 *   scheduler
 *----------------------------------------------------------------*/
void *schedulerTiempo(void *arg) {
	struct parametros *p;
	p = (struct parametros *)arg;
	int id = p -> tid;
	printf("Soy un Scheduler con número [%d] \n", id);
}
void *schedulerEvento(void *arg) {
	struct parametros *p;
	p = (struct parametros *)arg;
	int id = p -> tid;
	printf("Soy un Scheduler con número [%d] \n", id);

}
/*----------------------------------------------------------------- 
 *   Queue
 *----------------------------------------------------------------*/
struct Queue* createQueue() 
{ 
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue)); 
    queue->front = queue->size = 0; 
  
    // This is important, see the enqueue 
    queue->rear = BUFFER_MAX - 1; 
    return queue; 
} 

// Queue is full when size becomes 
// equal to the capacity 
int isFull(struct Queue* queue) 
{ 
    return (queue->size == BUFFER_MAX); 
} 
  
// Queue is empty when size is 0 
int isEmpty(struct Queue* queue) 
{ 
    return (queue->size == 0); 
} 
  
// Function to add an item to the queue. 
// It changes rear and size 
void enqueue(struct Queue* queue, struct PCB pcb) 
{ 
    if (isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1) 
                  % BUFFER_MAX; 
    queue->arr_pcb[queue->rear] = pcb; 
    queue->size = queue->size + 1; 
    //printf("%d enqueued to queue\n", pcb.id); 
} 
  
// Function to remove an item from queue. 
// It changes front and size 
struct PCB dequeue(struct Queue* queue) 
{ 
    if (isEmpty(queue)) {
		struct PCB pcbNulo;
		pcbNulo.quantum=0;
		return pcbNulo;
	} 
    struct PCB pcb = queue->arr_pcb[queue->front]; 
    queue->front = (queue->front + 1) 
                   % BUFFER_MAX; 
    queue->size = queue->size - 1; 
    return pcb; 
} 
  
// Function to get front of queue 
struct PCB front(struct Queue* queue) 
{ 
    if (isEmpty(queue)) { 	
        struct PCB pcbNulo;
		pcbNulo.quantum=0;
		return pcbNulo; 
	}
    return queue->arr_pcb[queue->front]; 
} 
  
// Function to get rear of queue 
struct PCB rear(struct Queue* queue) 
{ 
    if (isEmpty(queue)) { 	
        struct PCB pcbNulo;
		pcbNulo.quantum=0;
		return pcbNulo; 
	}
    return queue->arr_pcb[queue->rear]; 
} 

/*----------------------------------------------------------------- 
 *   mensajes
 *----------------------------------------------------------------*/

int mensaje_error(char *s) {
    printf("***error*** %s\n",s);
    exit(-1);
}