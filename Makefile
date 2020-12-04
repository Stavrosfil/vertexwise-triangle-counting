CC=g++
MPICC=mpicc
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang++
# CILKCC=clang++
CFLAGS= -O3 -w

BUILD_DIR=build
SRC_DIR=src
SOURCES := $(shell find $(SRC_DIR) -name '*.c')

$(info $(shell mkdir -p $(BUILD_DIR)))


default: all

serial: 
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/main $(SOURCES)

openmp: 
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/main $(SOURCES) -fopenmp

cilk:
	$(CILKCC) $(CFLAGS) -o $(BUILD_DIR)/main $(SOURCES) -fopencilk

pthreads:
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/main $(SOURCES) -lpthread

mpi:
	$(MPICC) $(CFLAGS) -o $(BUILD_DIR)/main $(SOURCES) -lpthread

.PHONY: clean

all: openmp

# @printf "\n** Testing pthreads\n\n"
# ./hello_threads
# @printf "\n** Testing OpenMP\n\n"
# ./hello_openmp
# @printf "\n\n** Testing MPI\n\n"
# mpirun ./hello_mpi
test:
	@printf "\n** Testing\n\n"
	./build/main

clean:
	rm -rf $(BUILD_DIR)
