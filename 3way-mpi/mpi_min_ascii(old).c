#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close

int NUM_THREADS;

char find_line_min(char *line, int line_length);
bool find_file_min_chars(int *result, char **lines, int total_lines);
int get_file_size(char *filename);

#define FILE_PATH "/homes/zachterrell57Proj4/3way-mpi/test/small_wiki_dump.txt"

char find_line_min(char *line, int line_length)
{
    int min = INT32_MAX;
    // printf("This is the line: %s", line);
    int i;
    for (i = 0; i < line_length - 1; i++)
    {
        // printf("The character is: %d\n", (int)line[i]);
        if ((int)line[i] < min && (int)line[i] > 32) // this could be changed but I do not know if we ignore control characters
        {
            min = (int)line[i];
        }
    }
    return min;
}

bool find_file_min_chars(int *result, char **lines, int total_lines) // add total lines here at some point
{
    int line_length;
    line_length = 0;        
    for (int line_number = 0; line_number < total_lines; line_number++)
    {
        line_length = strlen(lines[line_number]);
        result[line_number] = (int)(find_line_min(lines[line_number], line_length));        
    }
    return true;
}

int main(int argc, char *argv[])
{
    int i, rc, numtasks, rank;
    MPI_Status Status;

    int file, total_lines;

    char *buffer_start;

    char **lines;

    int *count;     

    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS)
    {        
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    NUM_THREADS = numtasks;

    if (rank == 0)
    {        
        // MPI Barrier
        // 
        file = open(FILE_PATH, O_RDONLY);        
        int file_size = get_file_size(FILE_PATH);        

        char *file_content = (char *)malloc(file_size);
        read(file, file_content, file_size);                
        buffer_start = NULL;
        char *line = NULL;
        lines = malloc(file_size);

        line = strtok_r(file_content, "\n", &buffer_start);            
        lines[0] = line;        
        total_lines = 1;
            
        while ((line = strtok_r(NULL, "\n", &buffer_start)))
        {            
            lines[total_lines] = line;            
            total_lines++;
        }
        count = (int *)malloc(total_lines);
    }

    int result[total_lines];

    MPI_Bcast(lines, total_lines , MPI_CHAR, 0, MPI_COMM_WORLD);

    find_file_min_chars(result, lines, total_lines);

    MPI_Reduce(result, count, total_lines, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int i;
        for (i = 0; i < total_lines; i++)
        {
            printf("%d: %d\n", i, result[i]);
        }

        close(file);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}

int get_file_size(char *filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}