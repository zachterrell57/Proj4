#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <mpi.h>

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close

#define LINE_COUNT_MAX 1000 //maximum line count
#define FILE_PATH "/homes/dan/625/wiki_dump.txt" //path to wiki dump

unsigned char find_min_ascii(char *line);
int get_file_size(char *filename);

//finds the minimum ascii value of a given line
unsigned char find_min_ascii(char *line){
    unsigned char min = 0xFF;
    int line_length = strlen(line);    
    int i;
    for (i = 0; i < line_length - 1; i++)
    {        
        if ((line[i] < min) && ((line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122)))
        {
            min = line[i];
        }
    }
    return min;
}

int main(int argc, char *argv[]){

    //clock to gauge runtime
    double time_spent = 0.0;
 
    clock_t begin = clock();

    int rc, numtasks, rank;
    MPI_Status Status;

    int file;
    
    //initialize MPI
    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS)
    {        
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    //if master process
    if(rank == 0){
        int line = 0;
        //wait for children processes
        MPI_Barrier(MPI_COMM_WORLD);  
        unsigned char result;      
        while(line<LINE_COUNT_MAX){                    
            for(int i = 1; i < numtasks && line < LINE_COUNT_MAX; i++, line++){                                                
                //recieve work done by slave process         
                MPI_Recv(&result, sizeof(unsigned char), MPI_UNSIGNED_CHAR, i, line, MPI_COMM_WORLD, MPI_STATUS_IGNORE);                   
                printf("%d : %d\n", line, (int)result);
            }
        }

        clock_t end = clock();
 
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

        printf("DATA, Runtime: %f\n", time_spent);

    }
    //process is a slave process
    else{
        file = open(FILE_PATH, O_RDONLY);     
        int file_size = get_file_size(FILE_PATH);
        char *buffer = malloc(file_size);       
        read(file, buffer, file_size);
        close(file);
          
        char **line_ptrs = calloc(LINE_COUNT_MAX+1, sizeof(char *));        
        char *next = NULL; 
        line_ptrs[0] = strtok_r(buffer, "\n", &next);        
        char *line;
        int total_lines = 1;
        while((line = strtok_r(NULL, "\n", &next)) && total_lines < LINE_COUNT_MAX)
        {            
            line_ptrs[total_lines] = line;     
            total_lines++;       
        }  
        // wait for all processes to sync
        MPI_Barrier(MPI_COMM_WORLD);            
        int numworkers;
        //iterate through file and find min ascii value
        for(int i = rank - 1, numworkers = numtasks - 1; line_ptrs[i] != NULL && i < LINE_COUNT_MAX; i += numworkers){            
            int min_ascii = find_min_ascii(line_ptrs[i]);            
            MPI_Send(&min_ascii, sizeof(unsigned char), MPI_UNSIGNED_CHAR, 0, i, MPI_COMM_WORLD);
        }        
        free(buffer);
        free(line_ptrs);
    }
    //end mpi
    MPI_Finalize();
    return 0;
}

//helper function for getting file size
int get_file_size(char *filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}