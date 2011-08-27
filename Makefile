DIRS=PAPC_a PAPC_b PAPC_c

all: run run_

%:
	-for d in $(DIRS); do ($(MAKE) -C $$d $@); done

.PHONY: all
