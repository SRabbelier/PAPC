all: PAPC_b

PAPC_b: PAPC_b.o
	gcc -o $@ $< -lm

run: PAPC_b
	./PAPC_b

.PHONY: run
