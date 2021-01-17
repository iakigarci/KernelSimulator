#include <pthread.h>

#include "definitions.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "thread.h"

#include "queue.h"

/*----------------------------------------------------------------- 
 *   clock
 *----------------------------------------------------------------*/

void *kernelClock(void *arg) {
	struct parametros *ptr, p;
	ptr = (struct parametros *)arg;
	int id = p.tid;
	int frec = p.frec;
	
	int count;
	while(1) {
		count++;
		if (count == 100000*frec)
		{
			count=0;
			pthread_mutex_lock(&mutexC);
			clockTime++;
			//printf("  CLOCK[%d] \n", clockTime);
			pthread_mutex_unlock(&mutexC);	
			decrementarQ_ListaPCB();
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

void *loader(void *arg) {
	struct parametros *ptr, p;
	ptr = (struct parametros *)arg;
	int frec = p.frec;
	char path[22];
	int idFichero=0;
	char buffer[255];
	bool salir = false;
	while (1)
	{
		// Abrir carpeta y leer todos los ficheros
		snprintf(path,22,"prometeo/prog%03d.elf",idFichero);
		FILE * fichero = fopen(path,"r");
		int nLineas=2;
		while (fgets(buffer,120,fichero)!=NULL)	// Obtener nnumero de lineas
		{
			nLineas++;
		}
		int nMarcos;
		if (nLineas % 64 == 0) // Obtener marcos necesarios
		{
			nMarcos = nLineas / 64 ;
		}else {
			nMarcos = 1 + nLineas / 64 ;
		}
		salir = false;
		while(!salir) {
			if (nMarcos >= marcosDisp)
			{
				salir = true;
				struct PCB pcb;
				//asignar prioridad y quantum
				sleep(frec);
				pcb.id=idFichero;
				pcb.prioridad = rand() % (3+1-0) + 0;
				pcb.quantum = rand() % (100+1-1) + 1;
							
				// Se lee el ficher, inicializando mm de pcb y leyendo las lineas
				rewind(fichero);
				fgets(buffer,120,fichero); // Avanza uno
				char * linea = strtok(buffer," "); // Separamos el texto
				linea = strtok(NULL," ");
				pcb.mm.code = strtol(linea,&linea,16);
				fgets(buffer,120,fichero);
				char * linea = strtok(buffer," "); // Separamos el texto
				linea = strtok(NULL," ");
				pcb.mm.data = strtol(linea,&linea,16);

				// Tabla de paginas
				pcb.mm.pgb = malloc(sizeof(long)*nMarcos);
				int pPGB = 0;
				for (int j = 0; j < sizeMemoria; j+=256)
				{
					if (memoriaFisica[j]==0)
					{
						pcb.mm.pgb[pPGB] = j;
						pPGB++;
						if (pPGB==nMarcos)
						{
							break;
						}	
					}
				}
				int nIns,pgbAct = 0;
				long posMemoria = pcb.mm.pgb[0];
				long lineaLeida;

				while (fgets(buffer,120,fichero)!=NULL)	// Se lee lineas desde la 3ra y se rellaena memoria
				{
					if (nIns==63) // Marco lleno
					{
						pgbAct++;
						nIns=0;
						posMemoria = pcb.mm.pgb[pgbAct];
					}
					
					lineaLeida = strtol(buffer,NULL,16);
					memoriaFisica[posMemoria]=lineaLeida;
					posMemoria+=4;
					nIns++;
				}

				switch (pcb.prioridad)	// Se introduce el PCB en cola de prioridades
				{
					case 0:
						enqueue(queue0_ptr,pcb);
						break;
					case 1:
						enqueue(queue1_ptr,pcb);
						break;
					case 2:
						enqueue(queue2_ptr,pcb);
						break;
					case 3:
						enqueue(queue3_ptr,pcb);
						break;
					default:
						break;
				}

				if (idFichero>49) // TODO
				{
					idFichero=0;
				}
			}else{ // Esperar hasta que se liberen los marcos
				sleep(15);
			}
		}
		
		fclose(fichero);
	}
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