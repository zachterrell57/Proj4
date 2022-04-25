#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// #include "openmp_min_ascii.c" for local parallel testing

#include "openmp_min_ascii.h"

int main(int argc, char **argv)
{
    if (argc < 2) // should be three but for buffering
    {
        printf("ERROR: %s <filename> <lines>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // FILE *file;
    // file = fopen("/homes/schoggatt/CIS520/Proj4/3way-openmp/test/output", "a");

    // select algorithm based on the input from the user and print if it completes

    // if(!file)
    // {
    //     printf("ERROR: Could not find readme.md and cannot write output.\n");
    // }

    printf("File name: %s\n", argv[1]);
    printf("File length: %s\n", argv[2]);
    int total_lines = atoi(argv[2]);

    int result[total_lines];

    // find_file_min_chars(result, argv[1], total_lines);

    find_file_min_chars(result, "/homes/dan/625/wiki_dump.txt", 10);

    //printf("------------------------\n");

    int i;
    for(i = 0; i < 10; i++)
    {
        printf("%d : %d\n", i, result[i]);
    }

    //printf("------------------------\n");

    // fclose(file);

    return EXIT_SUCCESS;
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