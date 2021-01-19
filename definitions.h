#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define DELAY_TIMER     5
#define BUFFER_MAX      5
#define WAITING_TO_EXIT 50
#define MAX_PCB			100
#define CLOCK_DEFAULT	1
#define TIMER_DEFAULT	1


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

typedef struct { 
  pthread_t tid; 
} identif_t; 

struct parametros {
	int tid;
	int frec;
};

enum scheduler_m { timer, event } scheduler_flag;

typedef struct mm {
	long code;
	long data;
	long * pgb;
} mm;

typedef struct TLB {
	long virtual;
	long fisica;
} TLB;

typedef struct pcb_status {
	long arr_registr[16];
	long IR;	// ultima instruccion
	long PC;	// direccion virtual de IR
	struct TLB TLB;
} pcb_status;

/** ESTRUCTURAS CPU **/
typedef struct PCB {
	int id;
	int quantum;
	int prioridad; // 0..3	
	struct mm mm;
	struct pcb_status pcb_status;
} PCB; 

typedef struct Queue { 
	int front, rear, size; 
	struct PCB arr_pcb[BUFFER_MAX]; 
} Queue;  

typedef struct core_thread 
{
	bool is_process;
	PCB t_pcb;
	long arr_registr[16];
} c_thread;

typedef struct cpu_core
{
   c_thread arr_th;
} core;

typedef struct cpu
{
   core arr_core;
} cpu_t;

struct cpu *arr_cpu;
pthread_mutex_t mutexT, mutexC, mutexPCB, mutexMemoria;
long * memoriaFisica;
int sizeMemoria, marcosDisp, marcosMax;
int clockTime, priorityTime, timer_flag;

struct Queue *queue0_ptr, queue0;
struct Queue *queue1_ptr, queue1;
struct Queue *queue2_ptr, queue2;
struct Queue *queue3_ptr, queue3;

#endif // DEFINITIONS_H