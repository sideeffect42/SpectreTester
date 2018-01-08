CFLAGS = -pipe -Wall -g

O ?= 0 # Optimisation level

BINDIR = bin

BINS = $(addprefix $(BINDIR)/,spectreO0 spectreO1 spectreO2 spectreO3 cache_timer)

SPECTRE_BIN = $(BINDIR)/spectreO$(O)
CACHE_TIMER_BIN = $(BINDIR)/cache_timer


.PHONY: all clean dist
.PHONY: spectre cache_timer

all: $(SPECTRE_BIN) $(CACHE_TIMER_BIN)

spectre: $(SPECTRE_BIN)
cache_timer: $(CACHE_TIMER_BIN)

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

$(BINDIR)/cache_timer: cache_timer.c spectre_archs.h spectre_intrinsics.h $(BINDIR)/
	$(CC) $(CFLAGS) -O$(O) -o '$@' cache_timer.c
