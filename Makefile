CFLAGS=-O2 -Wall -Wextra -Wpedantic --std=c99 -lSDL2 -lSDL2_ttf -lm

default: all

all: bin/ray

remake: clean all

bin/ray: src/*.c
	gcc src/*.c $(CFLAGS) -o bin/ray

clean:
	rm -f bin/*


