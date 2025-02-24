TOP_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
DMAKE = $(TOP_DIR)/docker/dmake

APPBIN  := ./bin/TSAnalyzer

all: 
	$(DMAKE) -m -C src all

run: all
	$(APPBIN)

.PHONY: init
init:
	chmod +x $(DMAKE)
	mkdir -p $(TOP_DIR)/bin
	$(DMAKE) -b

.PHONY: clean
clean:
	rm -f $(TOP_DIR)/bin/*
	$(DMAKE) -r "make -C src clean"
