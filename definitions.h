#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DELAY_TIMER     5
#define BUFFER_MAX      5
#define WAITING_TO_EXIT 15
#define NUM_CPU 		2
#define NUM_CORE		2
#define MAXTHREAD       5
#define MAX_PCB			100

typedef struct { 
  pthread_t tid; 
} identif_t; 

struct parametros {
	int tid;
	int frec;
};

enum scheduler_m { timer, event } scheduler_flag;

/** ESTRUCTURAS CPU **/
typedef struct PCB {
	int id;
	int quantum;
	int prioridad; // 0..3
}PCB; 

typedef struct Queue { 
	int front, rear, size; 
	struct PCB arr_pcb[BUFFER_MAX]; 
}Queue;  

typedef struct core_thread 
{
	bool is_process;
	struct PCB t_pcb;
} c_thread;

typedef struct cpu_core
{
   c_thread arr_th[MAXTHREAD];
} core;

typedef struct cpu
{
   core arr_core[NUM_CORE];
} cpu_t;

pthread_mutex_t mutexT, mutexC, mutexPCB;
int clockTime, priorityTime, timer_flag;
struct cpu arr_cpu[NUM_CPU];

struct Queue *queue0_ptr, queue0;
struct Queue *queue1_ptr, queue1;
struct Queue *queue2_ptr, queue2;
struct Queue *queue3_ptr, queue3;

#endif // DEFINITIONS_H