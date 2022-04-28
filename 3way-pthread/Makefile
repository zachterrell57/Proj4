all: output clean

output: pthread_min_ascii.o
	gcc -Wall -o pthread_min_ascii -lpthread pthread_min_ascii.c

pthread_min_ascii.o: pthread_min_ascii.c
	gcc -c pthread_min_ascii.c

clean:
	rm *.o 