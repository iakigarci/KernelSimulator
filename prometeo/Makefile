CFLAGS = -Wall -ggdb

SRC = *.c
HEAD = *.h

prometeo: $(SRC) $(HEAD)
	gcc -o $@ $(SRC) $(CFLAGS)  

.PHONY: clean
clean:
	rm -f *.o prometeo

