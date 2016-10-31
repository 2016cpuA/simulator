CC=gcc
FLAGS=-Wall -g

simulator: main.c assemble.o sim_binary.o simulator.o readline.o list.o instructs.o fetch.o
	$(CC) $(FLAGS) -o $@ $^
sim_binary.o: sim_binary.c assemble.o readline.o list.o instructs.o fetch.o
	$(CC) $(FLAGS) -c $< 
simulator.o: simulator.c assemble.o readline.o list.o instructs.o fetch.o
	$(CC) $(FLAGS) -c $< 
assemble.o:assemble.c readline.o list.o instructs.o fetch.o
	$(CC) $(FLAGS) -c $<
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
