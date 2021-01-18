#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DELAY_TIMER     5
#define BUFFER_MAX      5
#define WAITING_TO_EXIT 15
#define NUM_CPU 		3
#define NUM_CORE		2
#define MAXTHREAD       5
#define MAX_PCB			100
#define CLOCK_DEFAULT	100
#define TIMER_DEFAULT	100
#define MEMORY_SIZE_DEFAULT	8

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
}mm;

/** ESTRUCTURAS CPU **/
typedef struct PCB {
	int id;
	int quantum;
	int prioridad; // 0..3
	long arr_registr[16];
	struct mm mm;
}PCB; 

typedef struct Queue { 
	int front, rear, size; 
	struct PCB arr_pcb[BUFFER_MAX]; 
}Queue;  

typedef struct core_thread 
{
	bool is_process;
	struct PCB t_pcb;
	long arr_registr[16];
} c_thread;

typedef struct cpu_core
{
   c_thread arr_th[MAXTHREAD];
} core;

typedef struct cpu
{
   core arr_core[NUM_CORE];
} cpu_t;

typedef struct configuration_t {
    unsigned int  virtual_bits;
    unsigned int  offset_bits;
    unsigned int  offset_mask;
    unsigned int  num_page_bits;
    unsigned int  pages;
    unsigned int  max_lines;
    char		  *prog_name;
    unsigned int  first_number;
    unsigned int  how_many;
} configuration_t;


pthread_mutex_t mutexT, mutexC, mutexPCB;
int clockTime, priorityTime, timer_flag;
struct cpu arr_cpu[NUM_CPU];
struct configuration_t conf;
long * memoriaFisica;
int sizeMemoria, marcosDisp, marcosMax;



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
#endif // DEFINITIONS_H