#!/bin/sh
#SBATCH --job-name test
#SBATCH --nodes 1
#SBATCH --tasks-per-node=4

echo "4 task,0,0,0"
mpirun --mca btl_base_warn_component_unused 0  q1 10 10
mpirun --mca btl_base_warn_component_unused 0  q1 10 00
mpirun --mca btl_base_warn_component_unused 0  q1 10 1000
mpirun --mca btl_base_warn_component_unused 0  q1 10 10000
mpirun --mca btl_base_warn_component_unused 0  q1 100 10
mpirun --mca btl_base_warn_component_unused 0  q1 100 100
mpirun --mca btl_base_warn_component_unused 0  q1 100 1000
mpirun --mca btl_base_warn_component_unused 0  q1 100 10000
mpirun --mca btl_base_warn_component_unused 0  q1 1000 10
mpirun --mca btl_base_warn_component_unused 0  q1 1000 100
mpirun --mca btl_base_warn_component_unused 0  q1 1000 1000
mpirun --mca btl_base_warn_component_unused 0  q1 1000 10000
mpirun --mca btl_base_warn_component_unused 0  q1 10000 10
mpirun --mca btl_base_warn_component_unused 0  q1 10000 100
mpirun --mca btl_base_warn_component_unused 0  q1 10000 1000
mpirun --mca btl_base_warn_component_unused 0  q1 10000 10000
