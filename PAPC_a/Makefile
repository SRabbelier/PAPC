CFLAGS=-O0 -g -fopenmp

all: PAPC_a

PAPC_a: main.o PAPC_a.o ../main.h
	gcc -o $@ $^ -lm $(CFLAGS)

PAPC_a_: ../main.o PAPC_a.o  ../main.h
	gcc -o $@ $^ -lm -lpthread $(CFLAGS)

run: PAPC_a
	./PAPC_a

run_: PAPC_a_
	./PAPC_a_

.PHONY: run
