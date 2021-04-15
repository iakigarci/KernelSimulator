#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>   
#include <time.h>    
#include <stdbool.h>
#include <limits.h> 
#include <getopt.h>
#include <math.h>
#include <string.h>

#include "definitions.h"

void inicializar();
void asignarPCB(struct PCB pPcb);
void decrementarQuantumYEjecutar();
void aumentarPrioridad();
int todosHilosOcupados();
void display_header();
void guardarRegistros(struct core_thread *ptrCoreT);
void volcarRegistros(struct core_thread *ptrCoreT);
void ejecutarInstruccion(struct core_thread *ptrCoreT);
void limpiarMarcos(struct PCB *ptrPCB);
int mensaje_error(char *s);
void imprimirEstructura();
struct Queue* createQueue() ;
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue) ;
struct PCB front(struct Queue* queue) ;
struct PCB rear(struct Queue* queue) ;
void enqueue(struct Queue* queue, struct PCB pcb) ;
struct PCB dequeue(struct Queue* queue);
void subirPrioridadColas(struct Queue* pQueue1, struct Queue* pQueue2);
void *kernelClock(void *arg); 
void *timerScheduler(void *arg); 
void *schedulerTiempo(void *arg);  
void *schedulerEvento(void *c_thread); 
void *loader(void *arg);
void printPCB(struct PCB* ptrPCB);
void queuePCB(struct PCB pPCB);
int MEMORY_SIZE_DEFAULT = 20;
int NUM_CPU 		= 2;
int NUM_CORE		= 4;
int MAXTHREAD       = 4;

int main(int argc, char *argv[]) {

	//	Inicialización de estructuras	
	identif_t idtimer, idclock, idloader, idscheduler;;
	
	//	Control de parametros de entrada
	int opt, long_index;
	struct parametros p1; //	clock
	struct parametros p2; //	timer
	struct parametros p3; //	loader

	static struct option long_options[] = {
        {"help",       no_argument,       0,  'h' },
		{"clock",	required_argument,	0,	'c' },
		{"frencuencia",	required_argument,	0,	'f'},
		{"p",	required_argument,	0,	'p'},
		{"n",	required_argument,	0,	'n'},
		{"t",	required_argument,	0,	't'},
		{"m",	required_argument,	0,	'm'},
        {0,            0,                 0,   0  }
    };

	while ((opt = getopt_long(argc, argv,":h:c:f:p:n:m:", 
                        long_options, &long_index )) != -1) {
      switch(opt) {
        case '?':
            printf ("Uso %s [OPTIONS]\n", argv[0]);
            printf ("  -c  --clock=NNN\t"
                "Frecuencia del clock [%d]\n", CLOCK_DEFAULT);
            printf ("  -t  --timer=NNN\t"
                "Frecuencia del timer [%d]\n", TIMER_DEFAULT);
			printf ("  -h, --help\t\t"
			"Ayuda\n");
			printf ("  -nCPU"
                "Numero de CPU [%d]\n", NUM_CPU);
            printf ("  -nC  --nCores=NNN\t"
                "Numero de cores/nucleos [%d]\n", NUM_CORE);
			printf ("  -nT  --nThreads=NNN\t"
                "Numero de threads/hilos [%d]\n", MAXTHREAD);
			printf ("  -m  --memoria=NNN\t"
                "Multiplicador de tamaño memoria físifca (2^m), mínimo 8[%d]\n", MEMORY_SIZE_DEFAULT);
            printf ("Ejemplos:\n");
            printf ("  ./kernel -c100 -t100  -nCPU2 -C2 -nT4 -m8\n");
            printf ("  ./kernel -nprog -f60 -l1000 -p1\n");
            printf ("  ./kernel -nprog -f61 -l20   -p60\n");
            exit (2);
        case 'c':   /* -c or --clock */ 
			p1.tid=idclock.tid;
			p1.frec=atoi(optarg);
            break;
        case 'f':   /* -f or --frecuenciaTimer */ 
            p2.tid=idtimer.tid;
			p2.frec=atoi(optarg);
            break; 
		case 'p':   /* -p or --procesador */
			NUM_CPU = atoi(optarg);
            break; 
		case 'n':   /* -n or --nucleo */ 
			NUM_CORE = atoi(optarg);
            break; 
		case 'h':   /* -h or --hilo */ 
			MAXTHREAD =atoi(optarg);
            break;
		case 'm':
			printf("m");
			if (atoi(optarg)<8)
			{
				mensaje_error("El multiplicador de la memoria tiene que ser mayor que 8");
			}else{
				MEMORY_SIZE_DEFAULT = atoi(optarg);
			}
			break;
        default:
            mensaje_error("Argumento no valido"); 
			break;
      } 
    } 
	
	printf("\n[KERNEL] Finalizando la config del programa...\n");
	display_header();
	sizeMemoria = pow(2.0,MEMORY_SIZE_DEFAULT); // 2^argumento
	inicializar();
	p3.tid=idloader.tid;
	//	Inicializacion de los parámetros que van a tener los hilos
	
	pthread_create(&(idclock.tid),NULL,kernelClock,(void *)&p1);
	pthread_create(&(idtimer.tid),NULL,timerScheduler,(void *)&p2);
	pthread_create(&(idloader.tid),NULL,loader,(void *)&p3);
	pthread_create(&(idscheduler.tid),NULL,schedulerTiempo,NULL);

	sleep(WAITING_TO_EXIT);
	printf("\n [KERNEL]Finalizando ejecución del programa por tiempo: %d s\n",WAITING_TO_EXIT);
	
	imprimirEstructura();
	return(0);
}

void display_header() {
	time_t rawtime;
	struct tm *ptm;
	rawtime = time(NULL);
	ptm = localtime(&rawtime);

	printf("╔══════════════════════════════════════════════╗\n");
	printf("║  SE·SO          %02d/%02d/%04d  %02d:%02d \n",
										ptm->tm_mday, 
										ptm->tm_mon+1, 
										ptm->tm_year+1900, 
										ptm->tm_hour, 
										ptm->tm_min);
	printf("║       ☼☼☼ CPU de Iñaki García ☼☼☼      \n");
	printf("║   Numero de CPU: %03d            			\n", NUM_CPU);
	printf("║   Numero de nucleos: %03d            		\n", NUM_CORE);
	printf("║   Numero de hilos: %03d            		\n", MAXTHREAD);
	printf("║   Frecuencia del clock: %03d          	\n", CLOCK_DEFAULT);
	printf("║   Frecuencia del timer: %03d         		\n", TIMER_DEFAULT);
	printf("║   Tamaño de la memoria física: %d         \n", (int)pow(2,MEMORY_SIZE_DEFAULT));
	printf("║   Tiempo de espera del programa: %d       \n", WAITING_TO_EXIT);
	printf("╚══════════════════════════════════════════════╝\n\n");
	//sleep(2);

	// TODO, desplegar numero de programas
} // ___display_header

/*----------------------------------------------------------------- 
 *   inicializar
 *----------------------------------------------------------------*/

void inicializar() {
	printf(" [KERNEL] Inicializando estructuras...");
	pthread_mutex_init(&mutexT, NULL);
	pthread_mutex_init(&mutexC, NULL);	//	Mutex Clock
	pthread_mutex_init(&mutexPCB, NULL);    //	Mutex para el acceso de las estructas de CPU, structuras PCB
	pthread_mutex_init(&mutexMemoria, NULL);
	clockTime=0;
	queue0_ptr = createQueue();
	queue1_ptr = createQueue();
	queue2_ptr = createQueue();
	queue3_ptr = createQueue();

	arr_cpu = malloc(sizeof(struct cpu)*NUM_CPU);
	for (int i = 0; i < NUM_CPU; i++)
	{
		arr_cpu[i].arr_core = malloc(sizeof(struct cpu_core)*NUM_CORE);
		for (int j = 0; j < NUM_CORE; j++)
		{
			arr_cpu[i].arr_core[j].arr_th = malloc(sizeof(struct core_thread)*MAXTHREAD);
		}
	}
	
	marcosDisp = sizeMemoria / 256;
	marcosMax = marcosDisp;
	memoriaFisica = malloc(sizeof(long)*sizeMemoria);

} // inicializar

void asignarPCB(struct PCB pPcb) {
	int i = 0;
	int j = 0;
	int k = 0;
	bool seguir = true;
	pthread_mutex_lock(&mutexPCB);
	while (i < NUM_CPU && seguir)
	{
		while (j < NUM_CORE && seguir)
		{
			while (k < MAXTHREAD && seguir)
			{
				if(arr_cpu[i].arr_core[j].arr_th[k].is_process) { // Se mira si hay proceso
					k++;
				}else{
					printf("[SCHEDULER] Metido [%d] quantum [%d] en [%d][%d][%d]",pPcb.id,pPcb.quantum, i,j,k);
					arr_cpu[i].arr_core[j].arr_th[k].t_pcb = pPcb;
					seguir = false;
					arr_cpu[i].arr_core[j].arr_th[k].is_process = true;
					volcarRegistros(&arr_cpu[i].arr_core[j].arr_th[k]);
				}
			}
			k=0;
			j++;
		}
		j=0;
		k=0;
		i++;
	}
	pthread_mutex_unlock(&mutexPCB);
	// if (seguir){
	// 	asignarPCB(pPcb);
	// }
}

void decrementarQuantumYEjecutar() {
	int i = 0;
	int j = 0;
	int k = 0;
	
	while (i < NUM_CPU)
	{
		while (j < NUM_CORE)
		{
			while (k < MAXTHREAD)
			{
				if (arr_cpu[i].arr_core[j].arr_th[k].is_process)
				{
					pthread_mutex_lock(&mutexPCB);
					if (arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum>0)
					{
						if (arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum==1)
						{
							arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum=0;
							arr_cpu[i].arr_core[j].arr_th[k].is_process=false;
							// Se mete el pcb otra vez en la cola con quantum a 0, y se copia estado
							guardarRegistros(&arr_cpu[i].arr_core[j].arr_th[k]);
							enqueue(queue0_ptr,arr_cpu[i].arr_core[j].arr_th[k].t_pcb);	
						}
						ejecutarInstruccion(&arr_cpu[i].arr_core[j].arr_th[k]);
						arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum--;
					}
					pthread_mutex_unlock(&mutexPCB);
				}
				k++;
			}
			k=0;
			j++;
		} 
		j=0;
		k=0;
		i++;
	}
}

// guardan los registros del hilo en el pcb
void guardarRegistros(struct core_thread *ptrCoreT) {
	//pthread_mutex_lock(&mutexPCB);
	for (int i = 0; i < 16; i++)
	{
		ptrCoreT->t_pcb.pcb_status.arr_registr[i] = ptrCoreT->arr_registr[i];
		ptrCoreT->arr_registr[i] = 0;
	}
	//pthread_mutex_unlock(&mutexPCB);
}

// se vuelcan los registros del pcb en el hilo
void volcarRegistros(struct core_thread *ptrCoreT) {
	//pthread_mutex_lock(&mutexPCB);
	for (int i = 0; i < 16; i++)
	{
		ptrCoreT->arr_registr[i] = ptrCoreT->t_pcb.pcb_status.arr_registr[i];
	}
	//pthread_mutex_unlock(&mutexPCB);
}

void aumentarPrioridad() { 
	subirPrioridadColas(queue1_ptr,queue0_ptr);
	subirPrioridadColas(queue2_ptr,queue1_ptr);
	subirPrioridadColas(queue3_ptr,queue2_ptr);
}

/**
 * 0	False
 * 1	True
**/

int todosHilosOcupados() {
	int i,j,k;
	while (i < NUM_CPU)
	{
		while (j < NUM_CORE)
		{
			while (k < MAXTHREAD)
			{
				if(arr_cpu[i].arr_core[j].arr_th[k].is_process) {
					k++;
				}else{
					return 0;
				}
			}
			k=0;
			j++;
		}
		j=0;
		k=0;
		i++;
	}
	return 1;
}

void ejecutarInstruccion(struct core_thread *ptrCoreT) {  
	//pthread_mutex_lock(&mutexPCB);
	long instruccion = ptrCoreT->t_pcb.pcb_status.PC;
	long codigo = (instruccion & 0xF0000000) >> 28;				// 0,1,2,F
	long registro = (instruccion & 0x0F000000) >> 24;				// Primer registro	
	long direccionAbsoluta = (instruccion & 0x00FFFFFF);			// Direccion virtual completa
	long offset = (direccionAbsoluta & 0x0000FF);					// Offset de la direccion virtual
	long direccionVirtual = (direccionAbsoluta & 0xFFFF00) >> 8;	// Direccion virtual
	
	printf("[INSTRUCCION] %lx",instruccion);
	printf("   >  Registros previos : [ %d | %d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |            \n",
										ptrCoreT->arr_registr[0],
										ptrCoreT->arr_registr[1],
										ptrCoreT->arr_registr[2],
										ptrCoreT->arr_registr[3],
										ptrCoreT->arr_registr[4],
										ptrCoreT->arr_registr[5],
										ptrCoreT->arr_registr[6],
										ptrCoreT->arr_registr[7],
										ptrCoreT->arr_registr[8],
										ptrCoreT->arr_registr[9],
										ptrCoreT->arr_registr[10],
										ptrCoreT->arr_registr[11],
										ptrCoreT->arr_registr[12],
										ptrCoreT->arr_registr[13],
										ptrCoreT->arr_registr[14],
										ptrCoreT->arr_registr[15]
	);

	if (codigo==0 || codigo==1)
	{
		long direccionFisicaAux;
		if (ptrCoreT->t_pcb.pcb_status.TLB.virtual != direccionVirtual) // Actualizar TLB o no
		{
			direccionFisicaAux = ptrCoreT->t_pcb.mm.pgb[direccionVirtual];
			ptrCoreT->t_pcb.pcb_status.TLB.virtual = direccionVirtual;
			ptrCoreT->t_pcb.pcb_status.TLB.fisica = ptrCoreT->t_pcb.mm.pgb[direccionVirtual];
		}else{
			direccionFisicaAux = ptrCoreT->t_pcb.pcb_status.TLB.fisica;
		}
		long direccionFisica = (direccionFisicaAux << 8) + offset;
		
		if (codigo==0)	// ld
		{
			pthread_mutex_lock(&mutexMemoria);
			ptrCoreT->arr_registr[registro] = memoriaFisica[direccionFisica];
			pthread_mutex_unlock(&mutexMemoria);
			
		}else{	// st
			pthread_mutex_lock(&mutexMemoria);
			memoriaFisica[direccionFisica] = ptrCoreT->arr_registr[registro];
			pthread_mutex_unlock(&mutexMemoria);
		}	
		
	}else if (codigo==2)	// add
	{
		long registro1 = instruccion & (0x00F00000) >> 20;
		long registro2 = instruccion & (0x000F0000) >> 16;
		ptrCoreT->arr_registr[registro] = ptrCoreT->arr_registr[registro1] + ptrCoreT->arr_registr[registro2];
	}else if (codigo==15)	// exit
	{
		ptrCoreT->is_process=false;
		limpiarMarcos(&ptrCoreT->t_pcb);
	}else{
		mensaje_error("Codigo de instruccion incorrecto");
	}

	printf("   > Codigo: %ld            			\n", codigo);
	printf("   > Registro: %ld            			\n", registro);
	printf("   > Direccion Absoluta: %lx           \n", direccionAbsoluta);
	printf("   > Direccion Virtual: %lx            \n", direccionVirtual);
	printf("   > Offset: %lx            			\n", offset);
	printf("   > Registros : [ %d | %d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |%d |            \n",
										ptrCoreT->arr_registr[0],
										ptrCoreT->arr_registr[1],
										ptrCoreT->arr_registr[2],
										ptrCoreT->arr_registr[3],
										ptrCoreT->arr_registr[4],
										ptrCoreT->arr_registr[5],
										ptrCoreT->arr_registr[6],
										ptrCoreT->arr_registr[7],
										ptrCoreT->arr_registr[8],
										ptrCoreT->arr_registr[9],
										ptrCoreT->arr_registr[10],
										ptrCoreT->arr_registr[11],
										ptrCoreT->arr_registr[12],
										ptrCoreT->arr_registr[13],
										ptrCoreT->arr_registr[14],
										ptrCoreT->arr_registr[15]
	);
	//pthread_mutex_unlock(&mutexPCB);
	//long num_inst = cpu[i].cores[j].procesos[w].IR;        
    //long dir_fis_offset = (dir_fis_sin_offset << 8) + (4 * num_inst);
    //cpu[i].cores[j].procesos[w].PC = memoria[dir_fis_offset];

	ptrCoreT->t_pcb.pcb_status.IR += 0x4;
	pthread_mutex_lock(&mutexMemoria);
	ptrCoreT->t_pcb.pcb_status.PC = memoriaFisica[(ptrCoreT->t_pcb.mm.pgb[ptrCoreT->t_pcb.pcb_status.IR >> 8 ] << 8 ) + (ptrCoreT->t_pcb.pcb_status.IR & 0xff)];
	pthread_mutex_unlock(&mutexMemoria);

}

void limpiarMarcos(struct PCB *ptrPCB) {
	pthread_mutex_lock(&mutexPCB);
	for (long i = 0; i < sizeof(ptrPCB->mm.pgb); i++)
	{
		long marco = ptrPCB->mm.pgb[i];
		for (long j = 0; j < 256; i++)
		{
			pthread_mutex_lock(&mutexMemoria);
			memoriaFisica[marco+j] = 0;
			pthread_mutex_unlock(&mutexMemoria);
		}
		marcosDisp++;
	}
	pthread_mutex_unlock(&mutexPCB);
}

/*----------------------------------------------------------------- 
 *   mensajes
 *----------------------------------------------------------------*/

int mensaje_error(char *s) {
	printf("***error*** %s\n",s);
	exit(-1);
}

void imprimirEstructura() {
	printf("Estructura \n");
	printf("Número de CPU [%d] \n", NUM_CPU);
	printf("Número de CORE [%d] \n", NUM_CORE);
	printf("Número de thread [%d] \n", MAXTHREAD);
	int i = 0;
	int j = 0;
	int k = 0;
	while (i < NUM_CPU)
	{
		printf("CPU [%d] \n", i);
		while (j < NUM_CORE)
		{
			printf("\t CORE [%d] \n", j);
			while (k < MAXTHREAD)
			{
				printf("\t \t T [%d]", k);
				printf("\t \t  PCB [%d]|[%d]|[%d] ", arr_cpu[i].arr_core[j].arr_th[k].t_pcb.id,arr_cpu[i].arr_core[j].arr_th[k].t_pcb.prioridad,arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum);
				printf("\n");
				k++;
			}
			k=0;
			j++;
		} 
		j=0;
		k=0;
		i++;
	}
	printQueue(queue0_ptr);
	printQueue(queue1_ptr);
	printQueue(queue2_ptr);
	printQueue(queue3_ptr);
}

/*----------------------------------------------------------------- 
 *   Queue
 *----------------------------------------------------------------*/
struct Queue* createQueue() 
{ 
	struct Queue* pQueue = (struct Queue*)malloc(sizeof(struct Queue)); 
	pQueue->front = pQueue->size = 0; 
  
	pQueue->rear = BUFFER_MAX - 1; 
	return pQueue; 
} 

int isFull(struct Queue* pQueue) 
{ 
	return (pQueue->size == BUFFER_MAX); 
} 
  
int isEmpty(struct Queue* pQueue) 
{ 
	return (pQueue->size == 0); 
} 
  
void enqueue(struct Queue* pQueue, struct PCB pcb) 
{ 
	if (isFull(pQueue)) 
		return; 
	pQueue->rear = (pQueue->rear + 1) 
				  % BUFFER_MAX; 
	pQueue->arr_pcb[pQueue->rear] = pcb; 
	pQueue->size = pQueue->size + 1; 
	//printf("%d enqueued to queue\n", pcb.id); 
} 
  
struct PCB dequeue(struct Queue* pQueue) 
{ 
	if (isEmpty(pQueue)) {
		struct PCB pcbNulo; 
		pcbNulo.quantum=0;
		return pcbNulo;
	} 
	struct PCB pcb = pQueue->arr_pcb[pQueue->front]; 
	pQueue->front = (pQueue->front + 1) 
				   % BUFFER_MAX; 
	pQueue->size = pQueue->size - 1; 
	return pcb; 
} 

struct PCB front(struct Queue* pQueue) 
{ 
	if (isEmpty(pQueue)) { 	
		struct PCB pcbNulo;
		pcbNulo.quantum=0;
		return pcbNulo; 
	}
	return pQueue->arr_pcb[pQueue->front]; 
} 
  
struct PCB rear(struct Queue* pQueue) 
{ 
	if (isEmpty(pQueue)) { 	
		struct PCB pcbNulo;
		pcbNulo.quantum=0;
		return pcbNulo; 
	}
	return pQueue->arr_pcb[pQueue->rear]; 
} 

void printQueue(struct Queue* pQueue)
{
	printf("Queue");
	printf("[ \t");
	struct PCB pcbAux;
	while (!isEmpty(pQueue))
	{
		pcbAux = dequeue(pQueue);
		printf("[%d]-[%d]-[%d]\t",pcbAux.id,pcbAux.prioridad,pcbAux.quantum);
	}
	printf("] \n");
}

/**
 * Sube cola 1 a cola 2
**/
void subirPrioridadColas(struct Queue* pQueue1, struct Queue* pQueue2) {
	bool seguir;
	while (seguir)
	{
		if (!isEmpty(pQueue1))
		{
			enqueue(pQueue2,dequeue(pQueue1));
		}else{ seguir=false; }
	}
}

/*----------------------------------------------------------------- 
 *   clock
 *----------------------------------------------------------------*/

void *kernelClock(void *arg) {
	struct parametros *ptr, p;
	ptr = (struct parametros *)arg;
	int id = p.tid;
	int frec = p.frec;
	printf("\n[CLOCK]: Iniciando...\n");
	int count;
	while(1) {
		count++;
		if (count == 100*frec)
		{
			count=0;
			pthread_mutex_lock(&mutexC);
			clockTime++;
			printf("\n[CLOCK]\n");
			pthread_mutex_unlock(&mutexC);	
			decrementarQuantumYEjecutar();
		}
	}
}

/*----------------------------------------------------------------- 
 *   timer
 *----------------------------------------------------------------*/

void *timerScheduler(void *arg) {
	struct parametros *ptr, p;
	ptr = (struct parametros *)arg;
	int id = p.tid;
	int frec = p.frec;
	while(1) {
		pthread_mutex_lock(&mutexC);
		if (clockTime>=frec)
		{
			clockTime=0;
			priorityTime++;
			//printf("  TIMER[%d] avisa\n", id);
			timer_flag=1;
			if (priorityTime>=10)
			{
				priorityTime=0;
				aumentarPrioridad();
				if(todosHilosOcupados()) {
					scheduler_flag = event;
				}
			}
		}
		pthread_mutex_unlock(&mutexC);	
	}
} 
/*----------------------------------------------------------------- 
 *   loader
 *----------------------------------------------------------------*/

void *loader(void *arg)
{
	struct parametros *ptr, p;
	ptr = (struct parametros *)arg;
	int frec = p.frec;
	char path[21];
	int idFichero = 0;
	char buffer[255];
	bool salir = false;
	while (1)
	{
		// Abrir carpeta y leer todos los ficheros
		snprintf(path, 21, "prometeo/prog%03d.elf", idFichero);
		FILE *fichero = fopen(path, "r");
		int nLineas = 2;
		if (fichero == NULL)
		{
			break;
		}else{		
			while (fgets(buffer, 120, fichero) != NULL) // Obtener numero de lineas
			{
				nLineas++;
			}
			int nMarcos;
			if (nLineas % 64 == 0) // Obtener marcos necesarios
			{
				nMarcos = nLineas / 64;
			}
			else
			{
				nMarcos = 1 + nLineas / 64;
			}
			salir = false;
			while (!salir)
			{
				if (nMarcos <= marcosDisp)
				{
					marcosDisp -= nMarcos;
					salir = true;
					struct PCB pcb;
					//asignar prioridad y quantum
					sleep(frec);
					pcb.id = idFichero;
					pcb.prioridad = rand() % (3 + 1 - 0) + 0;
					pcb.quantum = rand() % (100 + 1 - 1) + 1;

					// Se lee el ficher, inicializando mm de pcb y leyendo las lineas
					rewind(fichero);
					fgets(buffer, 120, fichero);	   // Avanza uno
					char *linea = strtok(buffer, " "); // Separamos el texto
					linea = strtok(NULL, " ");
					pcb.mm.code = strtol(linea, &linea, 16);
					fgets(buffer, 120, fichero);
					linea = strtok(buffer, " "); // Separamos el texto
					linea = strtok(NULL, " ");
					pcb.mm.data = strtol(linea, &linea, 16);

					// Tabla de paginas
					pcb.mm.pgb = (long *)malloc(sizeof(long) * nMarcos);
					int pPGB = 0;
					pthread_mutex_lock(&mutexMemoria);
					for (int j = 0; j < sizeMemoria; j += 256)
					{
						if (memoriaFisica[j] == 0)
						{
							pcb.mm.pgb[pPGB] = j >> 8;
							pPGB++;
							memoriaFisica[j] = 1; // Se introduce valor basura
							if (pPGB == nMarcos)
							{
								break;
							}
						}
					}
					
					long lineaLeida;
					int nIns, pgbAct = 0;
					long posMemoria = pcb.mm.pgb[0];
					fgets(buffer, 120, fichero);
					lineaLeida = strtol(buffer, NULL, 16);
					pcb.pcb_status.PC = lineaLeida;
					pcb.pcb_status.IR = pcb.mm.code;
					pcb.pcb_status.TLB.fisica = pcb.mm.pgb[0];
					pcb.pcb_status.TLB.virtual = 0x0;
					
					while (fgets(buffer, 120, fichero) != NULL) // Se lee lineas desde la 3ra y se rellena memoria
					{
						if (nIns == 63) // Marco lleno
						{
							pgbAct++;
							nIns = 0;
							posMemoria = pcb.mm.pgb[pgbAct] << 8;
						}

						lineaLeida = strtol(buffer, NULL, 16);
						memoriaFisica[posMemoria] = lineaLeida;
						posMemoria += 4;
						nIns++;
					}
					pthread_mutex_unlock(&mutexMemoria);
					printf("\n[LOADER]Introducido PCB: prog%03d.elf\n",idFichero);
					printPCB(&pcb);
					queuePCB(pcb);
				}
				else if (nMarcos > marcosMax)
				{
					printf("[LOADER] Programa con id [%d] es más grande que la memoria", idFichero);
					salir = true;
				}
				else
				{ // Esperar hasta que se liberen los marcos
					printf("[LOADER] Esperando a que se liberen los marcos...");
					sleep(5);
				}
			}
			fclose(fichero);
			idFichero++;
			sleep(2);
		}
	}
	printf("[LOADER] terminado");
}

void queuePCB(struct PCB pPCB) {
	switch (pPCB.prioridad)	// Se introduce el PCB en cola de prioridades
	{
		case 0:
			enqueue(queue0_ptr,pPCB);
			break;
		case 1:
			enqueue(queue1_ptr,pPCB);
			break;
		case 2:
			enqueue(queue2_ptr,pPCB);
			break;
		case 3:
			enqueue(queue3_ptr,pPCB);
			break;
		default:
			break;
	}
}

void printPCB(struct PCB* ptrPCB) {
	printf("   ├ ID: %03d            \n", ptrPCB->id);
	printf("   ├ Prioridad: %01d            \n", ptrPCB->prioridad);
	printf("   ├ Quantum: %03d            \n", ptrPCB->quantum);
	printf("   ├ IR: %lx            \n", ptrPCB->pcb_status.IR);
	printf("   ├ PC: %lx            \n", ptrPCB->pcb_status.PC);
	printf("   ├ TLB            \n");
	printf("   	├ Virtual: %5d | Fisica: %5d         \n", ptrPCB->pcb_status.TLB.virtual,ptrPCB->pcb_status.TLB.fisica);
	printf("   ├ Code: %lx        \n", ptrPCB->mm.code);
	printf("   ├ Data: %lx          \n", ptrPCB->mm.data);
	printf("   ├ PGB: %ld          \n", ptrPCB->mm.pgb[0]);
}

/*----------------------------------------------------------------- 
 *   scheduler
 *----------------------------------------------------------------*/
void *schedulerTiempo(void *arg) {
	struct parametros *ptr, p;
	ptr = (struct parametros *)arg;
	int id = p.tid;
	int frec = p.frec;
	bool seguir=true;
	while (1)
	{
		pthread_mutex_lock(&mutexT);
		if (timer_flag==1)
		{
			// printf("Soy un Scheduler por tiempo con número [%d] \n", id);
			timer_flag=0;
			seguir=true;
			while (seguir)
			{
				if (isEmpty(queue0_ptr))
				{
					if (isEmpty(queue1_ptr))
					{
						if (isEmpty(queue2_ptr))
						{
							if (isEmpty(queue3_ptr))
							{
								seguir=false;
							}else{asignarPCB(dequeue(queue3_ptr));}
						}else{asignarPCB(dequeue(queue2_ptr));}
					}else{asignarPCB(dequeue(queue1_ptr));}
				}else{asignarPCB(dequeue(queue0_ptr));}
			}
		}
		pthread_mutex_unlock(&mutexT);	
	}
}

void *schedulerEvento(void *c_ptr) {
	bool seguir=true;
	struct PCB pcb; 
	pthread_mutex_lock(&mutexT);
	while (seguir)
	{
		if (isEmpty(queue0_ptr))
		{
			if (isEmpty(queue1_ptr))
			{
				if (isEmpty(queue2_ptr))
				{
					if (isEmpty(queue3_ptr))
					{
						seguir=false;
					}else{pcb = dequeue(queue3_ptr);}
				}else{pcb = dequeue(queue2_ptr);}
			}else{pcb = dequeue(queue1_ptr);}
		}else{pcb = dequeue(queue0_ptr);}
	}
	pthread_mutex_unlock(&mutexT);
	if (seguir)
	{
		struct core_thread *c_ptrAux = (struct core_thread*)c_ptr;
		c_ptrAux->t_pcb=pcb;
		c_ptrAux->is_process=true;
	}
}