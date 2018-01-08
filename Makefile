CFLAGS = -pipe -Wall -g

O ?= 0 # Optimisation level

BINDIR = bin

BINS = $(addprefix $(BINDIR)/,spectreO0 spectreO1 spectreO2 spectreO3)
BIN = $(BINDIR)/spectreO$(O)


.PHONY: all clean dist

all:	$(BIN)

clean:
	$(RM) -r $(BINDIR)/

dist:
	tar czf SpectreTester.tar.gz Makefile *.c

%/:
	mkdir -p '$*'

$(BINS): Makefile

$(BINDIR)/spectreO0: spectre.c spectre_archs.h spectre_intrinsics.h $(BINDIR)/
	$(CC) $(CFLAGS) -O0 -o '$@' spectre.c

$(BINDIR)/spectreO1: spectre.c spectre_archs.h spectre_intrinsics.h $(BINDIR)/
	$(CC) $(CFLAGS) -O1 -o '$@' spectre.c

$(BINDIR)/spectreO2: spectre.c spectre_archs.h spectre_intrinsics.h $(BINDIR)/
	$(CC) $(CFLAGS) -O2 -o '$@' spectre.c

$(BINDIR)/spectreO3: spectre.c spectre_archs.h spectre_intrinsics.h $(BINDIR)/
	$(CC) $(CFLAGS) -O3 -o '$@' spectre.c
