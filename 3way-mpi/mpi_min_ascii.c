#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char find_line_min(char *line, int line_length);
bool find_file_min_chars(int *result, char **lines, int total_lines);
int get_file_size(char *filename);

#define FILE_PATH "/homes/zachterrell57Proj4/3way-openmp/test/small_wiki_dump.txt"

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

bool find_file_min_chars(int *result, char *filename, int total_lines) // add total lines here at some point
{
    FILE *file;
    char *line = (char *)malloc(2001);

    // file = fopen("/homes/dan/625/wiki_dump.txt", "r");
    file = fopen(filename, "r");

    int line_number; // max line is 10 for right now to limit run time and ensure accuracy
    int line_length;
    line_number = 0;
    line_length = 0;

    int i;
    for (i = 0; i < total_lines; i++)
    {
        fscanf(file, "%[^\n]\n", line);
        // if (err == EOF) // we cannot do this since openmp must not have a condition that can be broken upon parallel execution from my reading
        //     break;
        line_length = strlen(line);
        // printf("the line is: %s, The length is: %d\n", line, line_length);
        result[line_number] = (int)(find_line_min(line, line_length));
        // printf("%d\n", result[line_number]);
        line_number++;
    }

    fclose(file);
    return true;
}

main(int argc, char *argv[])
{
    int i, rc;
    int numtasks, rank;
    MPI_Status Status;

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
    }

    // Might need to multiply total_lines by the size of each line
    MPI_Bcast(buffer_start, total_lines, *MPI_CHAR, 0, MPI_COMM_WORLD);

    find_file_min_chars(result, lines, total_lines);

    // The second parameter might not be right? It is optional so we may not even need it
    MPI_Reduce(result, buffer_start, total_lines, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

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
    return 0;
}

int get_file_size(char *filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}