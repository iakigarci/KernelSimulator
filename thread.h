#ifndef THREAD_H
#define THREAD_H

void *kernelClock(void *arg); 
void *timerScheduler(void *arg); 
void *processGenerator(void *arg); 
void *schedulerTiempo(void *arg); 
void *schedulerEvento(void *c_thread);

#endif // THREAD_H
