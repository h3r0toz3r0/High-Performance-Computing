/**
  Question 2
  Compile: nvcc q2.cu -o q2
  Run: srun --pty --nodes 1 --job-name=interactive --partition=gpu --reservation=EECE5640 --gres=gpu:1 ./q2

    DEBUG:
        0   Print histogram
        1   Print file lines to ensure readability of file
        2   Print summation of coauthors per author
        3   Print max co-authors with respective author and execution time

**/

//      Include libraries       //
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <cuda.h>

//      Macros              //
#define DEBUG       3

//      CUDA Kernel             //
__global__ void sort(int *a, int *c, int n){     
    //  Get global threadID
    int id = blockIdx.x*blockDim.x+threadIdx.x; 

    //  Ensure we are within bounds   
    if (id < n){
        if(id == 0){
            c[0] = 0;
        }
        else{
            if(a[id] == a[id - 1]){
                c[id] = 1;
            }
            else{
                c[id] = 0;
            }
        }
    }
} 

//      Main Function           //   
int main( int argc, char* argv[] ){     
    //  Initialize local variables
    char c, string[100];
    int i;
    int N, count, buffer;
    float milliseconds = 0, milliseconds2 = 0;

    //  Timing using CUDA Events
    cudaEvent_t start, start2, midpoint, end;
    cudaEventCreate(&start);
    cudaEventCreate(&start2);
    cudaEventCreate(&midpoint);
    cudaEventCreate(&end);

    //  Start Clock
    cudaEventRecord(start);
    cudaEventRecord(start2);

    //  Allocate space
    count = 0;
    buffer = 5;
    int *chunk1 = (int *) malloc (buffer * sizeof(int));
    int *chunk2 = (int *) malloc (buffer * sizeof(int));

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
    chunk2[0] = 2;

    //  Ensure last value gets placed into arrays
    chunk1[317079] = 317079;
    chunk2[317079] = 317080;

    //  Close file
    fclose(file);

    //  Stop midpoint clock
    cudaEventRecord(midpoint);
    cudaEventElapsedTime(&milliseconds, start2, midpoint);

    //  Distribute data to GPU threads
    N = count;

    //  Allocate space for counting arrays
    int *d_chunk1 = (int *) malloc (N * sizeof(int));
    int *d_count1 = (int *) malloc ((N + 10) * sizeof(int));
    int *count_array1 = (int *) malloc ((N + 10) * sizeof(int));
    int *opposite_count_array = (int *) malloc (N * sizeof(int));

    //  Allocate memory for each vector on GPU
    cudaMalloc( &d_chunk1, (N + 1) * sizeof(int) );
    cudaMalloc( &d_count1, (N + 1) * sizeof(int) );

    //  Copy vectors to device
    cudaMemcpy( d_chunk1, chunk1, N * sizeof(int), cudaMemcpyHostToDevice );

    //  Initialize count arrays
    for(i = 0; i < N; i++){
        count_array1[i] = 0;
        opposite_count_array[i] = 0;
    }

    //  Copy vectors to device
    cudaMemcpy( d_count1, count_array1, N * sizeof(int), cudaMemcpyHostToDevice );

    //  Initialize variables for CUDA
    int blockSize, gridSize;
    blockSize = 1024;                                   //  Number of threads in each thread block
    gridSize = (int) ceil((float) N / blockSize);       //  Number of thread blocks in grid

    //  Execute the kernel
    sort<<<gridSize, blockSize>>>(d_chunk1, d_count1, N * sizeof(int));

    //  Copy back to host
    cudaMemcpy( count_array1, d_count1, N * sizeof(int), cudaMemcpyDeviceToHost );

    //  Count authors and place into opposite counting array
    int index = 0;
    for(i = 0; i < N; i++){
        if(count_array1[i] == 0){
            opposite_count_array[chunk1[index]] = 1;
        }
        else{
            opposite_count_array[chunk1[index]]++;
        }
        index++;
    }

    //  Count authors in chunk 2
    for(i = 0; i < N; i++){
        opposite_count_array[chunk2[i]]++;
    }

    //  End clock
    cudaEventRecord(end);
    cudaEventElapsedTime(&milliseconds2, start, end);

    //  Print results
    if(DEBUG == 2){
        printf("Histogram of authors:\n");
        for(i = 1; i < (N / 2); i++){
            if(opposite_count_array[i] != 0){
                printf("(%d, %d)\n", i, opposite_count_array[i]);
            }
        }
        printf("\n");
    }
    else if(DEBUG == 3){
        int max = 0;
        int max_index[N];
        for(i = 1; i < (N / 2); i++){
            if(opposite_count_array[i] != 0){
                if(opposite_count_array[i] >= max){
                    max = opposite_count_array[i];
                }
                max_index[i] = 0;
            }
        }
        for(i = 0; i < (N / 2); i++){
            if(opposite_count_array[i] == max)
                max_index[i] = i;
        }
        printf("\n\n Midpoint Execution Time (milliseconds): %10.8f\n", milliseconds);
        printf("Total Execution Time (milliseconds): %10.8f", milliseconds2);
        printf("\n\nThe largest number of co-authors is %d\n", max);
        printf("The author(s) is/are: ");
        for(i = 0; i < (N / 2); i++){
            if(max_index[i] != 0){
                printf("%d ", i);
            }
        }
        printf("\n\n");
    }
    else if (DEBUG == 0){
        for(i = 1; i < (N / 2); i++){
            count_array1[i] = 0;
            if(opposite_count_array[i] != 0){
                count_array1[opposite_count_array[i]]++;
            }
        }
        for(i = 0; i < (N / 2); i++){
            //  Author ID, co-author count
            if(count_array1[i] != 0)
            printf("%d,%d\n", i, count_array1[i]);
        }
    }

    //  Cleanly exit
    cudaFree(d_chunk1);
    cudaFree(d_count1);
    free(chunk1);
    free(chunk2);
    free(count_array1);
    free(opposite_count_array);

    return 0; 
} 
 
