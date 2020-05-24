/**
  Question 1
  Compile: mpicc q1.c -o q1
  Run: srun --pty --export=ALL --nodes=1 --tasks-per-node=3 mpirun --mca btl_base_warn_component_unused 0  q1
**/

//      Include libraries   //
#include "mpi.h"
#include <stdio.h> 
#include <stdlib.h>

//      Macros              //
#define DEBUG   1

//      Dartboard           //
double dartboard(int darts){
    //  Initialize local variables
    double x, y, random_x, random_y, pi; 
    int count = 0;  
    int dart_count = 1;

    //  Throw darts
    while (dart_count < darts){
        //  Randomizes between 0 and 1
        random_x = 1.0 * (rand() / (double) RAND_MAX);
        random_y = 1.0 * (rand() / (double) RAND_MAX);

        //  Darts hit board randomly
        x = (2.0 * random_x) - 1.0;
        y = (2.0 * random_y) - 1.0;    

        //  Count the number of darts in the circle
        if( 1 >= (x * x) + (y * y))   
            count++;

        //  Throw next dart
        dart_count++;
    }

    //  Calculate pi
    pi = ((double) count / (double) darts) * 4;

    //  Return the calulcated pi value for this task
    return pi;
} 

//      Main              //
int main (int argc, char *argv[]){
    //  Initialize local variables
    int world_rank, world_size, dart_count, reduce;
    double scratch_pad, average_pi = 0, temp_pi;

    int ROUNDS = atoi(argv[1]);
    int DARTS = atoi(argv[2]);

    //  Initialize MPI
    MPI_Init(NULL, NULL);

    //  Find world information
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //  Debug only
    if (DEBUG == 0)
        printf("Task %d starting...\n", world_rank);

    //  Initialize rounds thrown as zero
    dart_count = 0;

    //  Do work
    while (dart_count < ROUNDS){
        //  Throw darts
       scratch_pad = dartboard(DARTS);

       //   Sum values of pi across tasks
       reduce = MPI_Reduce(&scratch_pad, &temp_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        
       if (world_rank == 0) {
            //  Average the value of pi
            average_pi = ((average_pi * dart_count) + (temp_pi / world_size)) / (dart_count + 1); 
        } 
       dart_count++;   
    } 

    //  Print the actual value of pi
    if (world_rank == 0){
        //  Debug only
        if (DEBUG == 0){
            printf("\nQuestion 1: Calculate Pi the Dart Method\n");
            printf("-----------------------------------------\n");
            printf("\tNumber of Rounds: %d\n", ROUNDS);
            printf("\tNumber of Darts: %d\n", DARTS);
            printf("\tDart value of pi: %10.8f\n", average_pi);
            printf("\tActual pi: 3.1415926535897\n\n");
        }
        else{
            printf("%10.8f,%d,%d\n", average_pi,DARTS,ROUNDS);
        }
        
    }

    //  Cleanly exit
    MPI_Finalize();
}


