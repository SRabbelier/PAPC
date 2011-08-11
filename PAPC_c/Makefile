CFLAGS=-O0 -fopenmp

all: PAPC_c

PAPC_c: PAPC_c.o
	gcc -o $@ $< -lm $(CFLAGS)

run: PAPC_c
	./PAPC_c

.PHONY: run
