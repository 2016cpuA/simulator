CC=gcc
FLAGS=-Wall -g

readline: readline.c list.o instructs.o
	$(CC) $(FLAGS) -o $@ $^
list.o: list.c list.h
	$(CC) $(FLAGS) -c $<
instructs.o: instructs.c instructs.h
	$(CC) $(FLAGS) -c $<

clean:
	rm -f readline *~; rm -f *.o
