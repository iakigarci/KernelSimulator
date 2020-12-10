
OBJS	= kernel.o queue.o thread.o mensajes.o
SOURCE	= kernel.c queue.c thread.c mensajes.c
HEADER	= queue.h definitions.h thread.h mensajes.h 
OUT	= kernel.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread
# -g option enables debugging mode 
# -c flag generates object code for separate files


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# create/compile the individual files >>separately<<
kernel.o: kernel.c
	$(CC) $(FLAGS) kernel.c 

queue.o: queue.c
	$(CC) $(FLAGS) queue.c 

thread.o: thread.c
	$(CC) $(FLAGS) thread.c 

mensajes.o: mensajes.c
	$(CC) $(FLAGS) mensajes.c 


# clean house
clean:
	rm -f $(OBJS) $(OUT)
