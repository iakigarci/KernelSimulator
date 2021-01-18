#ifndef THREAD_H
#define THREAD_H

#include "definitions.h"

void *kernelClock(void *arg); 
void *timerScheduler(void *arg); 
void *schedulerTiempo(void *arg); 
void *schedulerEvento(void *c_thread);
void *loader(void *arg);

#endif // THREAD_H
