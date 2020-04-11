CC         = gcc
CFLAGS     = -Wall --std=c99 -O2 -mavx ##Remove the -mavx if you're compiling for a literal dinosaur
DEBUGFLAGS = -g 
LDFLAGS    = -lSDL2 -lSDL2_ttf -lm
OBJFILES   = $(patsubst %.c,%.o,$(wildcard ./src/*.c))
TARGET     = bin/ray

#$(info Compiling these objects\: [${OBJFILES}])

debug: CFLAGS += $(DEBUGFLAGS)
debug: all

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)


clean:
	rm -f $(OBJFILES) $(TARGET) 

default: all



