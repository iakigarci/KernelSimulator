
OBJS	= kernel.o 
SOURCE	= kernel.c 
HEADER	= definitions.h 
OUT	= kernel.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


kernel.o: kernel.c
	$(CC) $(FLAGS) kernel.c 

queue.o: queue.c
	$(CC) $(FLAGS) queue.c 

thread.o: thread.c
	$(CC) $(FLAGS) thread.c 

mensajes.o: mensajes.c
	$(CC) $(FLAGS) mensajes.c 

clean:
	rm -f $(OBJS) $(OUT)
