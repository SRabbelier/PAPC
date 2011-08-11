all: PAPC_c

PAPC_c: PAPC_c.o
	gcc -o $@ $< -lm

run: PAPC_c
	./PAPC_c

.PHONY: run
