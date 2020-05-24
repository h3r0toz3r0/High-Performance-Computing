//  Question 2: Divisible by 3/5 //
//  7 February 2020              //
//  Author: Anna DeVries         //

/*      Libraries                       */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector> 
#include <iostream> 

/*      Defs                            */
#define DEBUG true
#define PTHREADS 8
#define LARGEST 5000

/*      Globals                         */
sem_t mutex;
pthread_t threads[PTHREADS];
int numbers[LARGEST];
int count = 0;

/*      Struct                          */
struct arg_struct {
  int start;
  int end;
  std::vector<int> frontier;
} args;

/*      Utility Functions               */
void *f1(void *arguments){
  //  Protect critical section
  sem_wait(&mutex);

  //  Local Variables
  struct arg_struct *args = (struct arg_struct *) arguments;
  int i;
  int start, end, value;
  int INT_NUM = LARGEST / PTHREADS;

  //  For debuging only
  if(DEBUG == true){
    printf("Frontier:\n");
    for(auto num : args -> frontier){
      printf("%d ", num);
    }
    printf("\n");
  }

  //  Pop off value from frontier
  value = args -> frontier[0];
  args -> frontier.erase(args -> frontier.begin());

  //  Initialize start and end points
  start = value * INT_NUM;
  end = start + INT_NUM;

  //  Check if divisible by 3 and/or 5
  for(i = start; i < end; i++){
    if( (i + 1) % 3 == 0 || (i + 1) % 5 == 0){
      numbers[i] = 1;
      count++;
    }
  }

  //  Unlock critical section
  sem_post(&mutex);
}

/*      Main                            */
int main(){
  //  Local Variables
  int i;

  //  Initialize array to be false
  for(i = 0; i < LARGEST; i++){
    numbers[i] = 0;
  }

  //  Initialize frontier of values
  for(i = 0; i < PTHREADS; i++){
    args.frontier.push_back(i);
  }

  //  Initialize sem
  sem_init(&mutex, 0, 1);

  //  Create pthread and perform ops
  for(i = 0; i < PTHREADS; i++){
    //  Initialize variables
    args.start = 0;
    args.end = 0;

    //  Create pthreads
    pthread_create(&threads[i], NULL, f1, (void *) &args);
  }

  //  Join pthreads
  for(i = 0; i < PTHREADS; i++){
    pthread_join(threads[i], NULL);
  }

  //  Print true values
  printf("Numbers divisible by 3 or 5 between 1 and %d:\n", LARGEST);
  for(i = 0; i < LARGEST; i++){
    if(numbers[i] == 1){
      printf("%d ", i + 1);
    }
  }
  printf("\n\n");
  printf("Count: %d\n\n", count);

  //  Clean up
  sem_destroy(&mutex);
  return 0;
}
