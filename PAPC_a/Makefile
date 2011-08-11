all: PAPC_a

PAPC_a: PAPC_a.o
	gcc -o $@ $< -lm

run: PAPC_a
	./PAPC_a

.PHONY: run
