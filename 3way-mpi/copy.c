#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close

#define LINE_COUNT_MAX 1000
#define FILE_PATH "/homes/zachterrell57Proj4/3way-mpi/test/small_wiki_dump.txt"

int find_min_ascii(char *line);
int get_file_size(char *filename);

int NUM_THREADS;

int find_min_ascii(char *line){
    int min = INT32_MAX;
    int line_length = strlen(line);    
    int i;
    for (i = 0; i < line_length - 1; i++)
    {        
        if ((int)line[i] < min && (int)line[i] > 32)
        {
            min = (int)line[i];
        }
    }
    return min;
}

int main(int argc, char *argv[]){

    unsigned char* result = malloc(LINE_COUNT_MAX);

    int rc, numtasks, rank;
    MPI_Status Status;

    int file;
    

    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS)
    {        
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
        int line = 0;
        MPI_Barrier(MPI_COMM_WORLD);        
        while(line<LINE_COUNT_MAX){                    
            for(int i = 1; i < numtasks && line < LINE_COUNT_MAX; i++, line++){  
                printf("here\n");                              
                MPI_Recv(&result, 1, MPI_UNSIGNED_CHAR, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);                                      
                printf("%d : %d\n", line, result[i]);
            }
        }
    }
    else{
        file = open(FILE_PATH, O_RDONLY); 
        int file_size = get_file_size(FILE_PATH);
        char *buffer = malloc(file_size);       
        read(file, buffer, file_size);

        MPI_Barrier(MPI_COMM_WORLD);                
        char **line_ptrs = calloc(LINE_COUNT_MAX, sizeof(char*));        
        char *next = NULL; 
        line_ptrs[0] = strtok_r(buffer, "\n", &next);        
        char *line;
        int total_lines;
        while(line = strtok_r(NULL, "\n", &next))
        {            
            line_ptrs[total_lines] = line;     
            total_lines++;       
        }        
        int numworkers;
        for(int i = rank - 1, numworkers = numtasks - 1; line_ptrs[i] != NULL && i < LINE_COUNT_MAX; i += numworkers){
            printf("while\n");
            int min_ascii = find_min_ascii(line_ptrs[i]);
            printf("%d\n", min_ascii);
            MPI_Send(&min_ascii, 1, MPI_UNSIGNED_CHAR, 0, rank, MPI_COMM_WORLD);
        }        
        free(buffer);
        free(line_ptrs);
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