CFLAGS=-O0 -fopenmp

all: PAPC_a

PAPC_a: PAPC_a.o
	gcc -o $@ $< -lm $(CFLAGS)

run: PAPC_a
	./PAPC_a

.PHONY: run
