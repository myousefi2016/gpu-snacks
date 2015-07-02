# All projects -> the solution.
SOLUTION ?= $(shell find src -name Makefile)

.PHONY: all
all: build
	@echo "Solution build finished!"

.PHONY: build
build: $(addsuffix .ph_build,$(SOLUTION))

.PHONY: clean
clean: $(addsuffix .ph_clean,$(SOLUTION))

.PHONY: clobber
clobber: $(addsuffix .ph_clobber,$(SOLUTION))

%.ph_build:
	+@$(MAKE) -C $(dir $*) $(MAKECMDGOALS)

%.ph_clean:
	+@$(MAKE) -C $(dir $*) clean

%.ph_clobber:
	+@$(MAKE) -C $(dir $*) clobber
