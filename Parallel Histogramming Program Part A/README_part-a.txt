Title
Readme for Question 2: Parallel Histogramming Programming Part A

Description
This program utilizes MPI to create a histogram. This program separates the input data evenly
among the nodes as (1/# classes)th of the input data values. The work is segregated among
the nodes using the MPI_Scatterv function. This function also allows the data to be segregated
unevenly given a non-divisible input. Each node then traverses its sub_data set and sums the
appropriate bin. After this, all node work is summed into the master node with the MPI_Reduce 
function. 

Usage
Turn on or off debug with macro 1 = true, 0 = false
Change the number of data input values and number of nodes in the command line.
Change the number of classes in line 14 (#define CLASS_NUM <int>).

mpicc q2.c -o q2
srun --pty --export=ALL --nodes=<NUM_NODES> --tasks-per-node=1 mpirun --mca btl_base_warn_component_unused 0  q2 <num_of_data_inputs>

Environment
I ran this program on the Discovery Cluster.

Testing
For testing this program, I turned debug on (#DEFINE DEBUG true). 
I then tested different values for class_num, number of nodes, and number of input data points.
I ensured that the values printed off represented an accurate histogram.