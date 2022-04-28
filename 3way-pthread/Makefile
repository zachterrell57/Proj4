all: output clean

output: mpi_min_ascii.o
	mpicc -Wall -o mp -lpthread pthread_min_ascii.c

pthread_min_ascii.o: pthread_min_ascii.c
	gcc -c pthread_min_ascii.c

clean:
	rm *.o 