CC=gcc
FLAGS=-Wall -g

simulator: simulator.c readline.o list.o instructs.o fetch.o
	$(CC) $(FLAGS) -o $@ $^ 
readline.o: readline.c list.o instructs.o
	$(CC) $(FLAGS) -c $< 
list.o: list.c list.h
	$(CC) $(FLAGS) -c $<
fetch.o: fetch.c instructs.h simulator.h
	$(CC) $(FLAGS) -c $<
instructs.o: instructs.c instructs.h simulator.h
	$(CC) $(FLAGS) -c $<

clean:
	rm -f readline simulator *~; rm -f *.o
