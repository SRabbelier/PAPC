CFLAGS=-O0 -fopenmp

all: PAPC_b

PAPC_b: main.o PAPC_b.o ../main.h
	gcc -o $@ $^ -lm $(CFLAGS)

PAPC_b_: ../main.o PAPC_b.o  ../main.h
	gcc -o $@ $^ -lm -lpthread $(CFLAGS)

run: PAPC_b
	./PAPC_b

run_: PAPC_b_
	./PAPC_b_

.PHONY: run
