CC          = gcc
CFLAGS      = -Wall -O2 -march=native ##Remove the -mavx if you're compiling for a literal dinosaur
DEBUGFLAGS  = -ggdb3
LDFLAGS     = -lSDL2 -lSDL2_ttf -lSDL2_image -lm
PROFILEFLAGS = -pg -g
OBJFILES    = $(patsubst %.c,%.o,$(wildcard ./src/*.c))
TARGET      = bin/ray
DISASM	    = objdump
DISASMFLAGS = -DlgCr -Mintel
DUMPFILE    = dump/ray-disasm

#$(info Compiling these objects\: [${OBJFILES}])

default: all

dump: $(DUMPFILE)

$(DUMPFILE):
	$(DISASM) $(DISASMFLAGS) $(TARGET) > $(DUMPFILE)

profile: CFLAGS += $(PROFILEFLAGS)
profile: all

debug: CFLAGS += $(DEBUGFLAGS)
debug: all

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)


clean:
	rm -f $(OBJFILES) $(TARGET) $(DUMPFILE)
