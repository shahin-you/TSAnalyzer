TOP_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
DMAKE = $(TOP_DIR)/docker/dmake

APPBIN  := ./bin/TSAnalyzer

.PHONY: init
init:
	chmod +x $(DMAKE)
	mkdir -p $(TOP_DIR)/bin
	$(DMAKE) -b

all: 
	$(DMAKE) -r "make -C src all"

run: all
	$(APPBIN)


.PHONY: clean
clean:
	rm -f $(TOP_DIR)/bin/*
	$(DMAKE) -r "make -C src clean"

#.PHONY: all run clean