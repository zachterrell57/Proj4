#!/bin/bash -l
#SBATCH --output 64_threads
#SBATCH --constraint=elves
#SBATCH --ntasks=64
#SBATCH --time=15
#SBATCH --mem-per-cpu=2g

./3way-mpi.sh