CFLAGS = -pipe -Wall -g

O ?= 0 # Optimisation level

BINS = spectreO0 spectreO1 spectreO2 spectreO3
BIN = spectreO$(O)

.PHONY: all clean dist

all:	$(BIN)

clean:
	$(RM) $(BINS)

dist:
	tar czf SpectreTester.tar.gz Makefile *.c

$(BINS): Makefile

spectreO0: spectre.c
	$(CC) $(CFLAGS) -O0 -o $@ spectre.c

spectreO1: spectre.c
	$(CC) $(CFLAGS) -O1 -o $@ spectre.c

spectreO2: spectre.c
	$(CC) $(CFLAGS) -O2 -o $@ spectre.c

spectreO3: spectre.c
	$(CC) $(CFLAGS) -O3 -o $@ spectre.c
