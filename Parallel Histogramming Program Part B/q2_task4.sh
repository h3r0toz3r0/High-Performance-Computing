#!/bin/sh
#SBATCH --job-name test
#SBATCH --nodes 4
#SBATCH --tasks-per-node=1

echo "Number of tasks,number of bins,number of elements,execution time"
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 10
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 50
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 100
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 500
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 1000
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 5000
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 10000
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 50000
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 100000
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 500000
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 1000000
mpirun --mca btl_base_warn_component_unused 0  q2a_class4 5000000

mpirun --mca btl_base_warn_component_unused 0  q2a_class25 10
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 50
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 100
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 500
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 1000
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 5000
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 10000
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 50000
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 100000
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 500000
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 1000000
mpirun --mca btl_base_warn_component_unused 0  q2a_class25 5000000

mpirun --mca btl_base_warn_component_unused 0  q2a_class100 10
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 50
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 100
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 500
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 1000
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 5000
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 10000
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 50000
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 100000
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 500000
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 1000000
mpirun --mca btl_base_warn_component_unused 0  q2a_class100 5000000

mpirun --mca btl_base_warn_component_unused 0  q2_class4 10
mpirun --mca btl_base_warn_component_unused 0  q2_class4 50
mpirun --mca btl_base_warn_component_unused 0  q2_class4 100
mpirun --mca btl_base_warn_component_unused 0  q2_class4 500
mpirun --mca btl_base_warn_component_unused 0  q2_class4 1000
mpirun --mca btl_base_warn_component_unused 0  q2_class4 5000
mpirun --mca btl_base_warn_component_unused 0  q2_class4 10000
mpirun --mca btl_base_warn_component_unused 0  q2_class4 50000
mpirun --mca btl_base_warn_component_unused 0  q2_class4 100000
mpirun --mca btl_base_warn_component_unused 0  q2_class4 500000
mpirun --mca btl_base_warn_component_unused 0  q2_class4 1000000
mpirun --mca btl_base_warn_component_unused 0  q2_class4 5000000

mpirun --mca btl_base_warn_component_unused 0  q2_class25 10
mpirun --mca btl_base_warn_component_unused 0  q2_class25 50
mpirun --mca btl_base_warn_component_unused 0  q2_class25 100
mpirun --mca btl_base_warn_component_unused 0  q2_class25 500
mpirun --mca btl_base_warn_component_unused 0  q2_class25 1000
mpirun --mca btl_base_warn_component_unused 0  q2_class25 5000
mpirun --mca btl_base_warn_component_unused 0  q2_class25 10000
mpirun --mca btl_base_warn_component_unused 0  q2_class25 50000
mpirun --mca btl_base_warn_component_unused 0  q2_class25 100000
mpirun --mca btl_base_warn_component_unused 0  q2_class25 500000
mpirun --mca btl_base_warn_component_unused 0  q2_class25 1000000
mpirun --mca btl_base_warn_component_unused 0  q2_class25 5000000

mpirun --mca btl_base_warn_component_unused 0  q2_class100 10
mpirun --mca btl_base_warn_component_unused 0  q2_class100 50
mpirun --mca btl_base_warn_component_unused 0  q2_class100 100
mpirun --mca btl_base_warn_component_unused 0  q2_class100 500
mpirun --mca btl_base_warn_component_unused 0  q2_class100 1000
mpirun --mca btl_base_warn_component_unused 0  q2_class100 5000
mpirun --mca btl_base_warn_component_unused 0  q2_class100 10000
mpirun --mca btl_base_warn_component_unused 0  q2_class100 50000
mpirun --mca btl_base_warn_component_unused 0  q2_class100 100000
mpirun --mca btl_base_warn_component_unused 0  q2_class100 500000
mpirun --mca btl_base_warn_component_unused 0  q2_class100 1000000
mpirun --mca btl_base_warn_component_unused 0  q2_class100 5000000