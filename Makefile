# Compiler
CC = g++

# Flags
CFLAGS = -Wall
OMPFLAGS = -fopenmp

# Targets
all: lab3 lab3_openmp

lab3: lab3.cpp
	$(CC) $(CFLAGS) lab3.cpp -o lab3

lab3_openmp: lab3_openmp.cpp
	$(CC) $(CFLAGS) $(OMPFLAGS) lab3_openmp.cpp -o lab3_openmp

run:
	@echo "Ejecutando lab3 (secuencial)..."
	./lab3
	@echo "\nEjecutando lab3_openmp (paralelo)..."
	./lab3_openmp

clean:
	rm -f lab3 lab3_openmp
