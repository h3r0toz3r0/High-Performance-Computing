/**
  Question 2
  Compile: mpicc q1.c -o q1
  Run: srun --pty --export=ALL --nodes=1 --tasks-per-node=3 mpirun --mca btl_base_warn_component_unused 0  q2
**/

//      Include libraries   //
#include "mpi.h"
#include <stdio.h> 
#include <stdlib.h>

//      Macros              //
#define DEBUG       1
#define CLASS_NUM   100
#define MAX         1000
#define MIN         0

//      Generate Input    //
void generate_input(int input_array[], int N, int world_rank){
    //  Initialize local variables
    int i;

    //  Create random input array on master node
    if(world_rank == 0){
        //  Initialize new randomization seed
        srand(time(NULL));

        //  DEBUG ONLY
        if(DEBUG == 1)
            printf("Random Input Array:\n\t");

        //  Initailize random data
        for( i = 0; i < N; i++){
            input_array[i] = rand() % ((MAX + 1) - MIN) + 1;
            //  DEBUG ONLY
            if(DEBUG == 1)
                printf("%d ", input_array[i]);
        }

        //  DEBUG ONLY
        if(DEBUG == 1)
            printf("\n\n");
    }

    return;
}

//      Generate Bins     //
void generate_bins(int class_maxes[], int temp_class_counts[], int class_counts[]){
    //  Initialize local variables
    int i, incr, remainder;

    //  Find bin size and remainder
    incr = (MAX - MIN) / CLASS_NUM;
    remainder = (MAX - MIN) % CLASS_NUM;

    //  Set bins
    for(i = 0; i < CLASS_NUM; i++){
        if(i == (CLASS_NUM - 1)){
            class_maxes[i] = incr + (incr * i) + remainder;
        }
        else{
            class_maxes[i] = incr + (incr * i);
        }
        temp_class_counts[i] = 0;
        class_counts[i] = 0;
        
    }
    return;
}

//      Main              //
int main (int argc, char *argv[]){
    //  Initialize local variables
    int world_rank, world_size, M, N, count, remainder;
    int i, j, k;
    double start, end; 
    
    //  Take user input for number of integers
    N = atoi(argv[1]);

    //  Initialize MPI
    MPI_Init(NULL, NULL);

    //  Find world information
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //  Local data count
    M = N / world_size;
    remainder = N % world_size;

    //  Create Scatterv count
    int scounts[CLASS_NUM];
    int displs[CLASS_NUM];
    k = 0;
    for(i = 0; i < CLASS_NUM; i++){
        if(i < remainder)
            scounts[i] = M + 1;
        else
            scounts[i] = M;
        displs[i] = k;
        k = k + scounts[i]; 
    }

    //  DEBUG ONLY
    if(world_rank == 0){
        if(DEBUG == 1){
            printf("\nQuestion 2, part a\n");
            printf("--------------------------------\n");
            printf("\nWorld Information:\n");
            printf("\tMax element: %d\n", MAX);
            printf("\tMin element: %d\n", MIN);
            printf("\tNumber of elements: %d\n", N);
            printf("\tNumber of bins: %d\n", CLASS_NUM);
            printf("\tNumber of elements per node: %d\n", M);
            printf("\tNumber of nodes: %d\n", world_size);
            printf("\n");
        }
    }

    //  Allocate space for arrays
    int* input_array = malloc(N * sizeof(int));
    int* temp_input = malloc(N * sizeof(int));
    int* class_maxes = malloc(CLASS_NUM * sizeof(int));
    int* temp_class_counts = malloc(CLASS_NUM * sizeof(int));
    int* class_counts = malloc(CLASS_NUM * sizeof(int));

    //  Generate input array
    generate_input(input_array, N, world_rank);

    //  Generate bins/classes
    generate_bins(class_maxes, temp_class_counts, class_counts);

    //  DEBUG ONLY
    if(world_rank == 0){
        if(DEBUG == 1){
            printf("Bin Info:\n");
            for(i = 0; i < CLASS_NUM; i++){
                printf("\tBin %d Max: ", i);
                printf("%d\n", class_maxes[i]);
            }
            printf("\n");
        }
    }

    //  Start clock
    start = MPI_Wtime();

    //  Distribute data to all tasks
    MPI_Scatterv(input_array, scounts, displs, MPI_INT, temp_input, (M + remainder), MPI_INT, 0, MPI_COMM_WORLD);

    //  Distribute data into bins
    count = 0;
    for(i = 0; i < (M + remainder); i++){
        if(temp_input[i] > 0){
            for(j = 0; j < CLASS_NUM; j++){
                if(temp_input[i] > class_maxes[j])
                    count++;
            }

            //  Increment appropriate bin count value
            temp_class_counts[count] += 1;
            count = 0;
        }
    }

    //  Sum all local count array to global count array
    for(i = 0; i < CLASS_NUM; i++){
        MPI_Reduce(&temp_class_counts[i], &class_counts[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    //  End clock
    end = MPI_Wtime();
    end = end - start;

    //  Print results
    if(world_rank == 0){
        if(DEBUG == 1){
            printf("Histogram (bin_num, count):\n");
            for(i = 0; i < CLASS_NUM; i++){
                printf("(%d, %d)\t", i, class_counts[i]);
            }
            printf("\n\nExecution Time (seconds): %10.8f", end);
            printf("\n\n");
        }
        else{
            //  Number of tasks, number of bins, number of elements, execution time
            printf("%d,%d,%d,%10.8f\n", world_size, CLASS_NUM, N, end);
        }
    }

    //  Cleanly exit
    free(input_array);
    free(temp_input);
    free(class_maxes);
    free(temp_class_counts);
    free(class_counts);

    MPI_Finalize();

    return 0;
}


