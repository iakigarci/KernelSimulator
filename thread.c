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
 *   process generator
 *----------------------------------------------------------------*/

void *processGenerator(void *arg) {
	struct parametros *ptr, p;
	ptr = (struct parametros *)arg;
	int id = p.tid;
	int frec = p.frec;
	int i=0;
	struct PCB pcb;
	while(1) {
		sleep(frec);
		pcb.id=i;
		pcb.prioridad = rand() % (3+1-0) + 0;
		pcb.quantum = rand() % (100+1-1) + 1;
		switch (pcb.prioridad)
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
		//printf("  Process Generator[%d] produce %02d %03d\n", id, pcb.id,pcb.quantum);
		i++;	
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