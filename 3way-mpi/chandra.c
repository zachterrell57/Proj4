unsigned char* result; <- instead of min_ascii

// main{
    mpi barrier(mpi comm world)

    while(line<line_count_max){
        for(i = 1; i < numtasks && line < line_count_max; i++, line++){
            MPIrec(&res.min_ascii, 1, mpi_unsigned_char, i, i, mpicommworld, mpistatusignore);
            printf(line num with min ascii)
        }
    }
// }

// slave{
    read into buffer
    open file
    mpi barrier
    line ptrs calloc
    line_ptrs[0] = split up string with "\n" (buf, "\n", &next)
    read other lines

    for(int i = rank - 1; numworkers = numtasks - 1; line_ptrs[i] != Null && i < line_count_max; i += numworkers){
        min_ascii = min_ascii
        MPI_Send(&min_ascii, 1, mpi_unsigned_char, 0, rank, mpicommworld)
    }

    free(buf)
    free(line_ptrs)

// }

MPI_Finalize();
return 0;
