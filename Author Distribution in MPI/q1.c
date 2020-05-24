/**
  Question 1
  Compile: mpicc q1.c -o q1
  Run: srun --pty --export=ALL --nodes=2 --tasks-per-node=1 mpirun --mca btl_base_warn_component_unused 0  q1

    DEBUG:
        0   Print histogram
        1   Print file lines to ensure readability of file
        2   Print summation of coauthors per author
        3   Print max co-authors with respective author and execution time

**/

//      Include libraries   //
#include "mpi.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

//      Macros              //
#define DEBUG       3

//      Main              //
int main (int argc, char *argv[]){
    //  Initialize local variables
    int world_rank, world_size;
    int i, j, k;
    char c, string[100];
    double start, end, midpoint; 

    //  Initialize MPI
    MPI_Init(NULL, NULL);

    //  Find world information
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //  DEBUG ONLY
    if(world_rank == 0){
        if(DEBUG != 0){
            printf("\nQuestion 1\n");
            printf("--------------------------------\n");
            printf("\nWorld Information:\n");
            printf("World Size: %d\n", world_size);
            printf("\n");
        }
    }

    //  Start clock
    start = MPI_Wtime();

    //  Allocate space
    int count = 0;
    int buffer = 5;
    int *chunk1 = (int *) malloc (buffer * sizeof(int));
    int *chunk2 = (int *) malloc (buffer * sizeof(int));

    //  On the master node: place all numbers as ints into arrays called chunk1 and chunk2
    if(world_rank == 0){
        //  Initiate file variables
        FILE *file;
        i = 0;

        //  Open file
        file = fopen("dblp-co-authors.txt","r");

        //  Iterate through each character of the file
        for(c = getc(file); c != EOF; c = getc(file)){
            //  If character is a space
            if (c == ' '){
                //  Initialize to 0
                chunk1[count] = 0;

                //  Convert string to int and place into array
                chunk1[count] = atoi(string);

                //  Replace string with 0s
                memset(string, 0, strlen(string)); 
            }
            //  If character is an end of line
            else if(c == '\n'){
                //  Initialize to 0
                chunk2[count] = 0;

                //  Convert string to int and place into array
                chunk2[count] = atoi(string);

                //  DEBUG Mode 2 ONLY
                if(DEBUG == 1){
                    printf("%d %d\n", chunk1[count], chunk2[count]);
                }

                //  Replace string with 0s
                memset(string, 0, strlen(string)); 

                //  Increment counter
                count++;

                //  Reallocate chunk array to ensure it is big enough 
                chunk1 = (int *) realloc(chunk1, sizeof(int) * (count + buffer));
                chunk2 = (int *) realloc(chunk2, sizeof(int) * (count + buffer));
            }

            //  If character is a information we want
            else{
                //  Add character to string array
                strncat(string, &c, 1); 
            }
        }

        //  Initialize zero element
        chunk1[0] = 0;
        chunk2[0] = 0;

        //  Ensure last value gets placed into arrays
        chunk1[317079] = 317079;
        chunk2[317079] = 317080;

        //  Close file
        fclose(file);

        //  Stop midpoint clock
        midpoint = MPI_Wtime();
    }

    //  Broadcast shared count 
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //  Distribute data to all tasks
    int N, M;
    N = count;
    M = N / world_size;

    //  Allocate space for counting arrays
    int *temp_chunk1 = (int *) malloc (M * sizeof(int));
    int *temp_count1 = (int *) malloc ((N + 10) * sizeof(int));
    int *count_array1 = (int *) malloc ((N + 10) * sizeof(int));
    int *count_array2 = (int *) malloc ((N + 10) * sizeof(int));
    int *temp_chunk2 = (int *) malloc (M * sizeof(int));
    int *temp_count2 = (int *) malloc ((N + 10) * sizeof(int));
    int *opposite_count_array = (int *) malloc (N * sizeof(int));

    //  Initialize arrays
    for(i = 0; i < N; i++)
        temp_count2[i] = 0;

    //  Distribute data to all tasks
    MPI_Scatter(chunk1, M, MPI_INT, temp_chunk1, M, MPI_INT, 0, MPI_COMM_WORLD);

    //  Count authors for chunk 1
    int index = 1;
    count = 1;

    //  Iterate through temp_chunk1 arrays 
    for(i = 0; i < M; i++){
        //  Determine if index value has changed from last time
        if(index != temp_chunk1[i]){
            temp_count1[index] = count;
            count = 1;
        }

        //  Otherwise, increase count
        else
            count++;
        
        //  Index is the value found at the array at i
        index = temp_chunk1[i];
    }

    //  Combine temp_chunks 
    for(i = 0; i < (N + 10); i++){
        MPI_Reduce(&temp_count1[i], &count_array1[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    //  Distribute data to all tasks
    MPI_Scatter(chunk2, M, MPI_INT, temp_chunk2, M, MPI_INT, 0, MPI_COMM_WORLD);

    //  Count authors for chunk 2
    for(i = 0; i < M; i++){
        index = temp_chunk2[i];
        temp_count2[index]++;
    }

    //  Combine temp_chunks 
    for(i = 0; i < N; i++){
        MPI_Reduce(&temp_count2[i], &count_array2[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    //  Sum count arrays together
    if(world_rank == 0){
        for(i = 0; i < N; i++){
            count_array1[i] = count_array1[i] + count_array2[i];
        }
    }

    //  Broadcast shared count 
    MPI_Bcast(&count_array1, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //  End clock
    end = MPI_Wtime();
    midpoint = midpoint - start;
    end = end - start;

    //  Print results
    if(world_rank == 0){
        if(DEBUG == 2){
            //  Print Number of co-authors per author
            printf("Number of co-authors per author\n");
            for(i = 0; i < N; i++){
                if(count_array1[i] != 0)
                    printf("(%d, %d)\n", i, count_array1[i]);
            }
            printf("\n");
        }
        else if (DEBUG == 3){
            //  Local variable
            int max = 0;

            //  Find max
            for(i = 0; i < N; i++){
                if(count_array1[i] > max)
                    max = count_array1[i];
            }

            printf("\nThe author(s) with the largest number of co-authors is/are: ");
            for(i = 0; i < N; i++){
                if(count_array1[i] == max)
                    printf("%d ", i);
            }
            printf("with %d co-authors\n", max);
            end = end * 1000;
            midpoint = midpoint * 1000;
            printf("Total Execution Time (milliseconds): %10.2f\n", end);
            printf("Midpoint Execution Time (milliseconds): %10.2f\n\n", midpoint);
        }
        else if (DEBUG == 0){
            for(i = 0; i < N; i++){
                opposite_count_array[i] = 0;
                if(count_array1[i] != 0){
                    opposite_count_array[count_array1[i]]++;
                }
            }
            for(i = 0; i < N; i++){
                //  Author ID, co-author count
                if(opposite_count_array[i] != 0)
                printf("%d,%d\n", i, opposite_count_array[i]);
            }
        }
    }

    //  Cleanly exit
    free(chunk1);
    free(chunk2);
    MPI_Finalize();

    return 0;
}

