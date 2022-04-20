#ifndef OPENMP_MIN_ASCII_H
#define OPENMP_MIN_ASCII_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include <stdbool.h>
#include <stdint.h>

    char find_line_min(char *line, int line_length);
    bool find_file_min_chars(int *result, char *filename);

#ifdef __cplusplus
}
#endif
#endif