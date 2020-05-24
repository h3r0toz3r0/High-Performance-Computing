#include <stdio.h> 
#include<stdlib.h>
#include <time.h>
#include <omp.h>    

#define M 512 

double CLOCK() {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC,  &t);
        return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

main(int argc, char **argv)
{
    int i,j,k,jj,kk,en;
    double start, finish, total, sum;
    float a[M][M], b[M][M], c[M][M];

/* Set the seed for the random number generators for the data */
    srand(145);

/* Initialize a, b and c */
    for (i=0; i<M; i++)
       for (j=0; j<M; j++)
          a[i][j] = (float)rand()/(float)RAND_MAX;

    for (i=0; i<M; i++)
       for (j=0; j<M; j++)
           b[i][j] = (float)rand()/(float)RAND_MAX;

    for (i=0; i<M; i++)
       for (j=0; j<M; j++)
           c[i][j] = 0.;


/* Start timing */
    start = CLOCK();

/* This is the only portion of the code you should modify to improve performance. */

//  Include OMP for parallizing matrix multiplication
#pragma omp parallel shared(a,b,c) private(i,j,k) 
   {
#pragma omp for  schedule(static)
    //  Swapped k and j to be more cache friendly
        //  k index on the inner-most loop causes cache misses in matrix b every iteration
        //  https://stackoverflow.com/questions/4300663/how-to-speed-up-matrix-multiplication-in-c
    for (i = 0; i < M; i++){
        for (k = 0; k < M; k++){
            for (j = 0; j < M; j++){
                c[i][j] = (c[i][j]) + ( (a[i][k]) * (b[k][j]) );
            }
        }
    }
}

    finish = CLOCK();
    
/* End timing */
    total = finish - start;
    printf("%4.2f,%f,%f,%f\n",total,c[0][0],c[63][63],c[511][511]);
    // printf("Time for the loop = %4.2f milliseconds\n", total);
    // printf("Element %d,%d = %f\n", 0, 0, c[0][0]); 
    // printf("Element %d,%d = %f\n", 63, 63, c[63][63]); 
    // printf("Element %d,%d = %f\n", 511, 511, c[511][511]); 
    return 0;
}

