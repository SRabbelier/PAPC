DIRS=PAPC_a PAPC_b PAPC_c

%:
	-for d in $(DIRS); do ($(MAKE) -C $$d $@); done

.PHONY: all
