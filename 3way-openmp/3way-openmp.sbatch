#!/bin/bash -l
#SBATCH --job-name pthread_min_ascii
#SBATCH --output pthread_min_ascii_%j
#SBATCH --time=6
#SBATCH --mem-per-cpu=2g
#SBATCH --ntasks=1

./openmp_min_ascii