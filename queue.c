#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "definitions.h"

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

// Queue is full when size becomes 
// equal to the capacity 
int isFull(struct Queue* pQueue) 
{ 
	return (pQueue->size == BUFFER_MAX); 
} 
  
// Queue is empty when size is 0 
int isEmpty(struct Queue* pQueue) 
{ 
	return (pQueue->size == 0); 
} 
  
// Function to add an item to the queue. 
// It changes rear and size 
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
  
// Function to remove an item from queue. 
// It changes front and size 
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
  
// Function to get front of queue 
struct PCB front(struct Queue* pQueue) 
{ 
	if (isEmpty(pQueue)) { 	
		struct PCB pcbNulo;
		pcbNulo.quantum=0;
		return pcbNulo; 
	}
	return pQueue->arr_pcb[pQueue->front]; 
} 
  
// Function to get rear of queue 
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