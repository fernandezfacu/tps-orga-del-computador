CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -O0 -ggdb -no-pie
LIBS:=-lm

all: cachesim

cachesim: cachesim.o optparser.o cache.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

cachesim.o: cachesim.c
	$(CC) $(CFLAGS) -c $^ -o $@

optparser.o: optparser.c
	$(CC) $(CFLAGS) -c $^ -o $@

cache.o: cache.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o
	rm -f cachesim
	

