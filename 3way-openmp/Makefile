all: output clean

output: openmp_min_ascii.o
	gcc -Wall -o openmp_min_ascii -fopenmp openmp_min_ascii.c

pthread_min_ascii.o: openmp_min_ascii.c
	gcc -c openmp_min_ascii.c

clean:
	rm *.o 