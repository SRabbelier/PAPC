CFLAGS=-O0 -fopenmp

all: PAPC_b

PAPC_b: PAPC_b.o
	gcc -o $@ $< -lm $(CFLAGS)

run: PAPC_b
	./PAPC_b

.PHONY: run
