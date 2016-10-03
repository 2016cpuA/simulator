CC=gcc
FLAGS=-Wall -g

readline: readline.c list.o
	$(CC) $(FLAGS) -o $@ $^
list.o: list.c list.h
	$(CC) $(FLAGS) -o $@ $<


clean:
	rm -f readline *~
