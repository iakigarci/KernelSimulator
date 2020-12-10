#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>   
#include <time.h>    
#include <stdbool.h>
#include <limits.h> 

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

/*----------------------------------------------------------------- 
 *   main
 * arg[1]	clockFrec
 * arg[2]	timerFrec
 * arg[3]	processFrec
 *----------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	
	printf("\n===========================================\n IInicio del programa de la asignatura de SO \n Creado por Iñaki García Noya \n===========================================\n");
	//	Inicialización de estructuras	
	identif_t idtimer, idclock, idprocessgenerator, idscheduler;;
	
	//	Control de parametros de entrada
	if (argc != 4)
	{
		mensaje_error("Los parametros sos: ./script frecuenciaClock frecuenciaTimer frecuenciaProcessGen \n Ejemplo: ./kernel 10 10 10 \n");
	}
	if(atoi(argv[1]) % 10 != 0) {
		mensaje_error("La frencuacia del reloj, parámetro 1, tiene que ser multiplo de 10");
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

	pthread_create(&(idclock.tid),NULL,kernelClock,(void *)&p1);
	pthread_create(&(idtimer.tid),NULL,timerScheduler,(void *)&p2);
	pthread_create(&(idprocessgenerator.tid),NULL,processGenerator,(void *)&p3);
	pthread_create(&(idscheduler.tid),NULL,schedulerTiempo,NULL);

	sleep(WAITING_TO_EXIT);
	printf("\nFinalizando ejecución del programa\n");
	
	imprimirEstructura();
	return(0);
}

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

