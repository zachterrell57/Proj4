#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "openmp_min_ascii.h"

int main(int argc, char **argv)
{
    if (argc < 2) // should be three but for buffering
    {
        printf("ERROR: %s <filename> <lines>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file;
    file = fopen("/homes/schoggatt/CIS520/Proj4/3way-openmp/test/output", "a");

    // select algorithm based on the input from the user and print if it completes

    if(!file)
    {
        printf("ERROR: Could not find readme.md and cannot write output.\n");
    }

    int result[10];

    find_file_min_chars(result, argv[0]);

    fprintf(file, "------------------------\n");

    int i;
    for(i = 0; i < 10; i++)
    {
        fprintf(file, "%d : %d\n", i, result[i]);
    }

    fprintf(file, "------------------------\n");

    fclose(file);

    return EXIT_SUCCESS;
}