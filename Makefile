CC=gcc
FLAGS=-Wall -g

readline: readline.c 
	$(CC) $(FLAGS) -o $@ $<

clean:
	rm -f readline *~
