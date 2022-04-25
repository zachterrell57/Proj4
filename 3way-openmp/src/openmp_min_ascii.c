// comment this out after testing it is in the header file

#include <stdbool.h>
#include <stdint.h>

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "openmp_min_ascii.h"

char find_line_min(char *line, int line_length)
{
    int min = INT32_MAX;
    //printf("This is the line: %s", line);
    int i;
    for (i = 0; i < line_length - 1; i++){
        //printf("The character is: %d\n", (int)line[i]);
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

    //file = fopen("/homes/dan/625/wiki_dump.txt", "r");
    file = fopen(filename, "r");

    int line_number; // max line is 10 for right now to limit run time and ensure accuracy
    int line_length;
    #pragma omp parallel private(line_length) shared(line_number)
    {
    line_number = 0;
    line_length = 0;

    #pragma omp for schedule(static, 10)
    //int i;
    for (int i = 0; i < total_lines; i++)
    {
        fscanf(file, "%[^\n]\n", line);
        // if (err == EOF) // we cannot do this since openmp must not have a condition that can be broken upon parallel execution from my reading
        //     break;
        line_length = strlen(line);
        // printf("the line is: %s, The length is: %d\n", line, line_length); 
        #pragma omp critical
        { 
        result[line_number] = (int)(find_line_min(line, line_length));
        // printf("%d\n", result[line_number]);
        line_number++;
        }
    }
    }

    fclose(file);
    return true;
}

// int main(int argc, char **argv)
// {
//     int result[10];

//     // find_file_min_chars(result, argv[1], total_lines);

//     find_file_min_chars(result, "/homes/dan/625/wiki_dump.txt", 1000);

//     for(int i = 0; i < 1000; i++)
//     {
//         printf("%d : %d\n", i, result[i]);
//     }

//     // fclose(file);

//     return EXIT_SUCCESS;
// }