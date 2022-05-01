#!/bin/bash -l
#SBATCH --job-name mpi_min_ascii
#SBATCH --output mpi_min_ascii_%j
#SBATCH --time=6
#SBATCH --mem-per-cpu=2g
#SBATCH --ntasks=1

mpirun -n 2 ./mpi_min_ascii