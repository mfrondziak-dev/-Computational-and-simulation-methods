CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
LDLIBS ?= -lgsl -lgslcblas -lm

.PHONY: all run plots clean

all: apr

apr: apr.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

run: apr
	./apr

plots: run
	gnuplot wykresy.gnuplot

clean:
	rm -f apr exp_cos.dat rational.dat oscillatory.dat
