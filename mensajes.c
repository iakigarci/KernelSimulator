#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "definitions.h"
#include "queue.h"

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
