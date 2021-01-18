#ifndef QUEUE_H
#define QUEUE_H

struct Queue* createQueue() ;
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue) ;
struct PCB front(struct Queue* queue) ;
struct PCB rear(struct Queue* queue) ;
void enqueue(struct Queue* queue, struct PCB pcb) ;
struct PCB dequeue(struct Queue* queue);
void subirPrioridadColas(struct Queue* pQueue1, struct Queue* pQueue2);

#endif // QUEUE_H
