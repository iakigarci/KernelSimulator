#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>   
#include <time.h>    
#include <stdbool.h>
#include <limits.h> 
#include <getopt.h>

#include "definitions.h"
#include "queue.h"
#include "thread.h"
#include "mensajes.h"

/** ---------------- **/

// void *schedulerEvento(struct core_thread c_thread);

void inicializar();
void asignarPCB(struct PCB pcb);
void decrementarQ_ListaPCB();
void aumentarPrioridad();
int todosHilosOcupados();
void display_header();
void guardarRegistros(struct PCB *ptrPCB, struct core_thread *ptrCoreT);

/*----------------------------------------------------------------- 
 *   main
 * arg[1]	clockFrec
 * arg[2]	timerFrec
 * arg[3]	processFrec
 *----------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	
	printf("\n===========================================\n Inicio del programa de la asignatura de SO \n Creado por Iñaki García Noya \n===========================================\n");
	//	Inicialización de estructuras	
	identif_t idtimer, idclock, idprocessgenerator, idscheduler;;
	
	//	Control de parametros de entrada
	int opt, long_index;
	struct parametros p1; //	clock
	struct parametros p2; //	timer
	struct parametros p3; //	processGenerator
	struct parametros p4; //	scheduler

	static struct option long_options[] = {
        {"help",       no_argument,       0,  'h' },
		{"clock",	required_argument,	0,	'c' },
		{"timer",	required_argument,	0,	't'},
		{"ncpu",	required_argument,	0,	'nCPU'},
		{"nc",	required_argument,	0,	'nC'},
		{"nT",	required_argument,	0,	'nT'},
		{"m",	required_argument,	0,	'm'},
        {0,            0,                 0,   0  }
    };
	
	while ((opt = getopt_long(argc, argv,":f:hl:n:p:", 
                        long_options, &long_index )) != -1) {
      switch(opt) {
        case 'h':   /* -h or --help */
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
        case 't':   /* -t or --timer */ 
            p2.tid=idtimer.tid;
			p2.frec=atoi(optarg);
            break; 
        case 'p':   /* -p or --programs */ 
            conf.how_many = atoi(optarg);
            break; 
		case 'nCPU':   /* -p or --programs */ 
            #undef NUM_CPU
			#define NUM_CPU atoi(optarg)
            break; 
		case 'nC':   /* -p or --programs */ 
            #undef NUM_CORE
			#define NUM_CORE atoi(optarg)
            break; 
		case 'nT':   /* -p or --programs */ 
            #undef MAXTHREAD
			#define MAXTHREAD atoi(optarg)
            break;
		case 'm':
			if (atoi(optarg)<8)
			{
				mensaje_error("El multiplicador de la memoria tiene que ser mayor que 8");
			}else{
				#undef MEMORY_SIZE_DEFAULT
				#define MEMORY_SIZE_DEFAULT atoi(optarg)
			}
			break;
        default:
            mensaje_error("Argumento no valido"); 
      } 
    } 
	display_header();
	sizeMemoria = pow(2,MEMORY_SIZE_DEFAULT); // 2^argumento
	inicializar();

	//	Inicializacion de los parámetros que van a tener los hilos
	
	p3.tid=idprocessgenerator.tid;
	p3.frec=atoi(argv[3]);

	pthread_create(&(idclock.tid),NULL,kernelClock,(void *)&p1);
	pthread_create(&(idtimer.tid),NULL,timerScheduler,(void *)&p2);
	pthread_create(&(idprocessgenerator.tid),NULL,processGenerator,(void *)&p3);
	pthread_create(&(idscheduler.tid),NULL,schedulerTiempo,NULL);

	sleep(WAITING_TO_EXIT);
	printf("\nFinalizando ejecución del programa\n");
	
	imprimirEstructura();
	return(0);
}

void display_header() {
	time_t rawtime;
	struct tm *ptm;
	rawtime = time(NULL);
	ptm = localtime(&rawtime);

	printf("╔═══════════════════════════════════╗\n");
	printf("║  SE·SO          %02d/%02d/%04d  %02d:%02d ║\n",
										ptm->tm_mday, 
										ptm->tm_mon+1, 
										ptm->tm_year+1900, 
										ptm->tm_hour, 
										ptm->tm_min);
	printf("║       ☼☼☼ CPU de Iñaki García ☼☼☼          ║\n");
	printf("║   Numero de CPU: %03d            ║\n", NUM_CPU);
	printf("║   Numero de nucleos: %03d            ║\n", NUM_CORE);
	printf("║   Numero de hilos: %03d            ║\n", MAXTHREAD);
	printf("║   Frecuencia del clock: %03d            ║\n", CLOCK_DEFAULT);
	printf("║   Frecuencia del timer: %03d            ║\n", TIMER_DEFAULT);
	printf("║   Tamaño de la memoria física: %03d            ║\n", pow(2,MEMORY_SIZE_DEFAULT));
	printf("╚═══════════════════════════════════╝\n\n");
	// TODO, desplegar numero de programas
} // ___display_header


/*----------------------------------------------------------------- 
 *   inicializar
 *----------------------------------------------------------------*/

void inicializar() {
	pthread_mutex_init(&mutexT, NULL);
	pthread_mutex_init(&mutexC, NULL);	//	Mutex Clock
	pthread_mutex_init(&mutexPCB, NULL);    //	Mutex para el acceso de las estructas de CPU, structuras PCB
	clockTime=0;
	queue0_ptr = createQueue();
	queue1_ptr = createQueue();
	queue2_ptr = createQueue();
	queue3_ptr = createQueue();

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
					printf("Metido [%d] quantum [%d] en [%d][%d][%d]",pPcb.id,pPcb.quantum, i,j,k);
					arr_cpu[i].arr_core[j].arr_th[k].t_pcb = pPcb;
					seguir = false;
					arr_cpu[i].arr_core[j].arr_th[k].is_process = true;
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

void decrementarQ_ListaPCB() {
	int i = 0;
	int j = 0;
	int k = 0;
	pthread_mutex_lock(&mutexPCB);
	while (i < NUM_CPU)
	{
		while (j < NUM_CORE)
		{
			while (k < MAXTHREAD)
			{
				if (arr_cpu[i].arr_core[j].arr_th[k].is_process)
				{
					if (arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum>0)
					{
						if (arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum==1)
						{
							arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum=0;
							arr_cpu[i].arr_core[j].arr_th[k].is_process=false;
							
							// Se mete el pcb otra vez en la cola con quantum a 0, y se copia estado
							guardarRegistros(&arr_cpu[i].arr_core[j].arr_th[k].t_pcb,&arr_cpu[i].arr_core[j].arr_th[k]);
							enqueue(queue0_ptr,arr_cpu[i].arr_core[j].arr_th[k].t_pcb);	
						}
						arr_cpu[i].arr_core[j].arr_th[k].t_pcb.quantum--;
					}
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
	pthread_mutex_unlock(&mutexPCB);
}

void guardarRegistros(struct PCB *ptrPCB, struct core_thread *ptrCoreT) {
	for (int i = 0; i < 16; i++)
	{
		ptrCoreT->arr_registr[i] = ptrPCB->arr_registr[16];
		ptrCoreT->arr_registr[i] = 0;
	}
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

void ejecutarInstruccion(struct PCB *ptrPCB, struct core_thread *ptrCoreT) {  
	
	long instruccion;
	long codigo = instruccion & (0xF0000000) >> 28;
	long registro = instruccion & (0x0F000000) >> 24;
	long direccionAbsoluta = instruccion & (0x0000FFFF);

	switch (codigo)
	{
	case '0':	// ld (load)
		ptrCoreT->arr_registr[registro] = 
		break;
	case '1':	// st 
		
		break;
	case '2':	// add
		
		break;
	case 'F':
		// Se limpia el thread y los marcos
		ptrCoreT->is_process=false;
		limpiarMarcos(ptrPCB);
		break;
	default:
		mensaje_error("Codigo de instruccion incorrecto");
		break;
	}	
}

void limpiarMarcos(struct PCB *ptrPCB){
	for (long i = 0; i < sizeof(ptrPCB->mm.pgb); i++)
	{
		long marco = ptrPCB->mm.pgb[i];
		for (long j = 0; j < 256; i++)
		{
			memoriaFisica[marco+j] = 0;
		}
		marcosDisp--;
	}
	
}