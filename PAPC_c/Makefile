CFLAGS=-O0 -fopenmp

all: PAPC_c

PAPC_c: main.o PAPC_c.o ../main.h
	gcc -o $@ $^ -lm -O0

PAPC_c_: ../main.o PAPC_c.o  ../main.h
	gcc -o $@ $^ -lm -lpthread $(CFLAGS)

run: PAPC_c
	./PAPC_c

run_: PAPC_c_
	./PAPC_c_

.PHONY: run
