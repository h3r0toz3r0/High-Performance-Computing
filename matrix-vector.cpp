//  Question 3: Matrix-Vector    //
//  7 February 2020              //
//  Author: Anna DeVries         //

/*      Libraries                       */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <omp.h>

/*      Defs                            */
#define DEBUG false
#define MATRIX_SIZE 512

/*      Globals                         */
int result[MATRIX_SIZE];
int matrix[MATRIX_SIZE][MATRIX_SIZE];
int vect[MATRIX_SIZE];

/*      Struct                          */
double CLOCK() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC,  &t);
  return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

/*      Utility Functions               */
int matrix_vector_multiply(){
  //  Local Variables
  int i, j;
  int tid;
  int thread_count;
  int partial_result;

//  OMP declaration 
#pragma omp parallel private(i)
{
  //  Set number of threads and get unique ThreadIDs
  tid = omp_get_thread_num();
  thread_count = omp_get_num_threads();

//  Utilize OpenMP functions to compute each thread
#pragma omp parallel for reduction(+: partial_result) 
  for(i = 0; i < MATRIX_SIZE; i++){
    for(j = 0; j < MATRIX_SIZE; j++){
      partial_result = result[i];
      partial_result += vect[j] * matrix[j][i];
      result[i] = partial_result;
    }
  }
}

//  Print unique threadIDs
printf("Unique ThreadIDs:\n");
for(i = 0; i < thread_count; i++){
  printf("Thread %d\n", i);
}
printf("\n");

  //  Exit function
  return 0;
}

/*      Main                            */
int main(){
  //  Local Variables
  int i, j;
  double start, finish, total;

  //  Starting message
  printf("Matric-Vector Multiply using OpenMP\nby Anna DeVries\n14 Feb 2020\n\n");

  //  Print system information
  printf("System Information\nNumber of processors configured: %d\nNumber of processsors available: %d\n\n", get_nprocs_conf(), get_nprocs());

  //  Initialize Matrix, Vector and Result; Randomize ints between 0 and 5
  for(i = 0; i < MATRIX_SIZE; i++){
    for(j = 0; j < MATRIX_SIZE; j++){
      matrix[i][j] = rand() % 5;
    }
  }
  for(i = 0; i < MATRIX_SIZE; i++){
    vect[i] = rand() % 5;
    result[i] = 0;
  }

  //  Debug purposes only
  if(DEBUG == true){
    printf("First Row of Matrix\n");
    for(i = 0; i < MATRIX_SIZE; i++){
      for(j = 0; j < 1; j++){
        printf("%d ", matrix[i][j]);
      }
    }
    printf("\n");
    printf("Vector\n");
    for(i = 0; i < MATRIX_SIZE; i++){
      printf("%d ", vect[i]);
    }
    printf("\n\n");
  }

  //  Start clock
  start = CLOCK();

  //  Call Matrix_Vector_Multiply
  matrix_vector_multiply();

  //  End clock
  finish = CLOCK();

  //  Print result[]
  printf("Resulting Matrix\n");
  for(i = 0; i < MATRIX_SIZE; i++){
    printf("%d ", result[i]);
  }
  printf("\n\n");

  //  Print Total timing
  total = finish - start;
  printf("Total Time: %4.2f milliseconds\n", total);

  //  Exit 
  return 0;
}
