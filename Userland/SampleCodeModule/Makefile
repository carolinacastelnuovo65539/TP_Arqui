include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)

SOURCES_ASM=$(wildcard asm/*.asm)

OBJECTS_ASM=$(SOURCES_ASM:asm/%.asm=obj/%.asm.o)

all: $(MODULE)


$(MODULE): $(OBJECTS_ASM) $(SOURCES)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(OBJECTS_ASM) $(SOURCES) -o ../$(MODULE)

obj/%.asm.o : asm/%.asm
	mkdir -p obj
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o obj

.PHONY: all clean print