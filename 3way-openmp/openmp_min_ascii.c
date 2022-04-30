// comment this out after testing it is in the header file

#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close

#define FILE_PATH "/homes/schoggatt/CIS520/Proj4/3way-openmp/test/partial_wiki_dump.txt"

// #include "openmp_min_ascii.h"

char find_line_min(char *line, int line_length);
bool find_file_min_chars(int *result, char **lines, int total_lines);
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

bool find_file_min_chars(int *result, char **lines, int total_lines)
{
    int line_length;
#pragma omp parallel private(line_length)
    {
        line_length = 0;

#pragma omp for 
        for (int line_number = 0; line_number < total_lines; line_number++)
        {
            line_length = strlen(lines[line_number]);
#pragma omp critical
            {
                result[line_number] = (int)(find_line_min(lines[line_number], line_length));
            }
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    omp_set_num_threads(5);

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
    while((line = strtok_r(NULL, "\n", &buffer_start)))
    {
        lines[total_lines] = line;
        total_lines++;
    }

    int result[total_lines];

    find_file_min_chars(result, lines, total_lines);

    int i;
    for (i = 0; i < total_lines; i++)
    {
        printf("%d: %d\n", i, result[i]);
    }

    close(file);

    return EXIT_SUCCESS;
}

int get_file_size(char *filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}