CFLAGS = -Wall -ggdb

SRC = *.c
# HEAD = *.h

heracles: $(SRC) # $(HEAD)
	gcc -o $@ $(SRC) $(CFLAGS)  

.PHONY: clean
clean:
	rm -f *.o heracles

