RPATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

.SILENT:
.PHONY: all clean init

all: build/*.so

init:
	if [ $(shell ./shell-check $(RPATH); echo $$?) -ne 0 ]; then \
		echo "Error: Current directory is not Makefile directory"; \
		exit 1; \
	fi

	mkdir build -p

build/*.so: init
	./compile-libs shared

build/*.a: init
	./compile-libs static

clean:
	rm -rf build
