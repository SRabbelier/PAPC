DIRS=PAPC_a PAPC_b PAPC_c

all:
	-for d in $(DIRS); do ($(MAKE) -C $$d); done

run:
	-for d in $(DIRS); do ($(MAKE) -C $$d run); done

.PHONY: all
