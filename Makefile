#
# In order to execute this "Makefile" just type "make"
#	A. Delis (ad@di.uoa.gr)
#

OBJS	= kernel.o queue.o
SOURCE	= kernel.c queue.c
HEADER	= queue.h definitions.h
OUT	= kernel.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread -lm
# -g option enables debugging mode 
# -c flag generates object code for separate files


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# create/compile the individual files >>separately<<
kernel.o: kernel.c
	$(CC) $(FLAGS) kernel.c 

queue.o: queue.c
	$(CC) $(FLAGS) queue.c 


# clean house
clean:
	rm -f $(OBJS) $(OUT)
