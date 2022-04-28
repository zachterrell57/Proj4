all: output clean

output: mpi_min_ascii.o
	mpicc -Wall -Werror mpi_min_ascii.c -o mpi

mpi_min_ascii.c: mpi_min_ascii.c
	mpicc mpi_min_ascii.c -o mpi

clean:
	rm *.o 