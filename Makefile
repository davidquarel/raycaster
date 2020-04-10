CFLAGS=-O2 -Wall --std=c99 -lSDL2 -lm

default: all

all: bin/ray

remake: clean all

bin/ray: src/*.c
	gcc src/*.c $(CFLAGS) -o bin/ray

clean:
	rm -f bin/*


