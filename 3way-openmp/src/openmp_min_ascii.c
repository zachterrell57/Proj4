#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openmp_min_ascii.h"

char find_line_min(char *line, int line_length)
{
    char min = 255;

    int i;
    for (i = 0; i < line_length - 1; i++)
        if (line[i] < min)
            min = line[i];

    return min;
}

bool find_file_min_chars(int *result, char *filename) // add total lines here at some point
{
    int line_number = 0, max_lines = 10; // max line is 10 for right now to limit run time and ensure accuracy
    int line_length = 0;
    FILE *file;
    char *line = (char *)malloc(2001);

    //file = fopen("/homes/dan/625/wiki_dump.txt", "r");
    file = fopen(filename, "r");

    int i, err;
    for (i = 0; i < max_lines; i++)
    {
        err = fscanf(file, "%[^\n]\n", line);
        if (err == EOF)
            break;
        line_length = strlen(line);
        result[line_number] = (int)find_line_min(line, line_length);
        line_number++;
    }

    fclose(file);
    return true;
}