// comment this out after testing it is in the header file

#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <time.h> 
#include <sys/resource.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>  // for open
#include <unistd.h> // for close

#define FILE_PATH "/homes/dan/625/wiki_dump.txt"
#define NUM_OF_THREADS 1 // change both in the batch file and here

typedef struct t_parameters
{
    int id;
    int total_lines;
    char **lines;
    int *result;
} Parameters;

char find_line_min(char *line, int line_length);
void * find_file_min_chars(void *args);
int get_file_size(char *filename);

char find_line_min(char *line, int line_length)
{
    int min = INT32_MAX;
    int i;
    for (i = 0; i < line_length - 1; i++)
    {
        if (((int)line[i] < min) && (((int)line[i] >= 65 && (int)line[i] <= 90) || ((int)line[i] >= 97 && (int)line[i] <= 122)))
        {
            min = (int)line[i];
        }
    }
    return min;
}

void * find_file_min_chars(void *args)
{
    Parameters *parameters = ((Parameters *) args);

    int line_length = 0;

    int line_number;
    for (line_number = parameters->id; line_number < parameters->total_lines; line_number += NUM_OF_THREADS)
    {
        line_length = strlen((parameters->lines)[line_number]);
        (parameters->result)[line_number] = (int)(find_line_min((parameters->lines)[line_number], line_length));
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    double time_spent = 0.0;
 
    clock_t begin = clock();

    int file = open(FILE_PATH, O_RDONLY);

    int file_size = get_file_size(FILE_PATH);

    char *file_content = (char *)malloc(file_size);
    read(file, file_content, file_size);

    char *buffer_start = NULL;
    char *line = NULL;
    char **lines = malloc(file_size);

    line = strtok_r(file_content, "\n", &buffer_start);

    lines[0] = line;

    int total_lines = 1;
    while ((line = strtok_r(NULL, "\n", &buffer_start)))
    {
        lines[total_lines] = line;
        total_lines++;
    }

    int result[total_lines];

    pthread_t threads[NUM_OF_THREADS];
    int i;
    for(i = 0; i < NUM_OF_THREADS; i++)
    {
        Parameters *parameters = malloc(sizeof(Parameters));
        parameters->id = i;
        parameters->total_lines = total_lines;
        parameters->lines = lines;
        parameters->result = result;
        
        pthread_create(&threads[i], NULL, find_file_min_chars, (void *) parameters);
    }

    for(i = 0; i < NUM_OF_THREADS; i++)
    {
        void * return_value;
        pthread_join(threads[i], &return_value);
    }

    for (i = 0; i < total_lines; i++)
    {
        printf("%d: %d\n", i, result[i]);
    }

    struct rusage r_usage;

    close(file);

    clock_t end = clock();
 
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    getrusage(RUSAGE_SELF, &r_usage);

    printf("DATA, Runtime: %f, Memory Usage: %ld\n", time_spent, r_usage.ru_maxrss);

    pthread_exit(NULL);

    return EXIT_SUCCESS;
}

int get_file_size(char *filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}