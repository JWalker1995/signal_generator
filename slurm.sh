#! /bin/bash
## -p partition (also works, see documentation for other alternatives)
#SBATCH --partition=TC
#SBATCH --job-name=signal_generator
#SBATCH --error=res_%j.err
#SBATCH --output=res_%j.out
#SBATCH --time=00:00:05
##  05 is an arbitrary limit for this course.
## --mem memory required per node
#SBATCH --mem=1G
#SBATCH --ntasks=4
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
module load openmpi/openmpi-1.10.2
## A bit redundant given my .bashrc
module list
mpirun ./signal_generator "$@"
exit 0
