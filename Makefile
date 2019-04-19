CFLAGS=-O2 -Wall --std=c99 -lSDL2 -lm
CC=gcc

default: all

all: bin/ray

remake: clean all

bin/ray: src/ray.c
	$(CC) src/ray.c $(CFLAGS) -o bin/ray

clean:
	rm -f bin/*


