CC=g++
MPICC=mpicc
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang++
CFLAGS=-O3 -w
BUILD_DIR=build

default: all

build_openmp: 
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/main src/mmio.c src/main.cpp -fopenmp

# hello_threads:
# 	$(CC) $(CFLAGS) -o hello_threads hello_threads.c -lpthread

# hello_mpi:
# 	$(MPICC) $(CFLAGS) -o hello_mpi hello_mpi.c

build_cilk:
	$(CILKCC) $(CFLAGS) -o $(BUILD_DIR)/main src/mmio.c src/main.cpp -fopencilk

.PHONY: clean

all: build_openmp test

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
	rm -f hello_openmp hello_mpi hello_cilk hello_threads 
