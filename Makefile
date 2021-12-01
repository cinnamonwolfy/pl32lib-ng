RPATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

.SILENT:
.PHONY: all clean init

all: build/*.so build/*.a

init:
	case $(shell $(RPATH)/shell-check $(RPATH); echo $$?) in \
		1) \
			echo "Error: Current directory is not Makefile directory"; \
			exit 1 \
			;; \
		2) \
			echo "Error: Variables have not been exported"; \
			exit 2 \
			;; \
	esac

	mkdir build -p

build/*.so: init
	./compile-libs -c ${SHARED_CFG}

build/*.a: init
	./compile-libs -c ${STATIC_CFG}

clean:
	rm -rf build
