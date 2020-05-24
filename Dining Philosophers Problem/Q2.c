//  Question 2: Prime Numbers    //
//  7 February 2020              //
//  Author: Anna DeVries         //

/*      Libraries                       */
#include <stdlib.h> 
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h> 
#include <time.h>

/*      Defs                            */
#define DEBUG false

/*      Globals                         */
pthread_mutex_t lock; 
bool *primes;

/*      Structs                         */
struct param {
    int num;
    int start;
    int end;
};

/*      Sieve Eratosthenes              */
// Sieve Eratosthenes code modified from https://www.geeksforgeeks.org/sieve-of-eratosthenes/ 
void* SieveOfEratosthenes(void *arg){
    //  Local Variables
    int p, i;
    struct param *args;
    args = (struct param *) arg;

    //  Assign Variables
    int start = args->start;
    int end = args->end;
    int t_num = args->num;

    //  DEBUG ONLY
    if(DEBUG == true)
        printf("ID: %d  Start: %d  End: %d\n", t_num, start, end); 

    //  Locate primes
    i = 0;
    for(p = 2; p <= end; p = i + 1){
        //  Must be within range 
        if(p * p >= start){
            i = p * p - 1;
        }
        else{
            i = start + p;
            i = i - start % p;
            i = i - 1;
        }
        //  Modify primes array 
        for(i = i; i <= end; i += p){
            primes[i] = 1;
        }
        i = p;
        while(primes[i] == 1){
            i++;
        }
    }
}

/*      Main                            */
int main( int argc, char *argv[]){ 
    //  Local variables  
    int i = 0; 
    int error;
    struct param *args;
    clock_t start, stop;
    double total;

    //  Check to ensure proper program execution
    if ( argc < 3 || argc > 4){
        printf("Usage: ./<filename> <num_of_threads> <largest_num>\n");
        return 1;
    }

    //  Set largest and threads variables
    int largest = atoi(argv[2]);
    int threads = atoi(argv[1]);

    //  Print welcome
    printf("Welcome to the Prime Number Cruncher\n");
    printf("Author: Anna DeVries\n");
    printf("1 Feb 2020\n\n");
    printf("Crunching for 0 to %d with %d threads......\n\n", largest, threads);

    //  Start clock
    start = clock();

    //  Initialize array of primes
    primes = (bool *) malloc(sizeof(bool) * largest);
    for(i = 0; i < largest; i++){
        primes[i] = 0;
    }

    //  Initialize pthread mutex
    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    }

    //  Distribute load with threads
    pthread_t tid[threads]; 
    int factor = largest / threads;
    int remainder = largest % threads;

    //  Divide and conquer
    args = (struct param *)malloc(sizeof(struct param) * threads);
    int j = 0;
    for(i = 0; i < threads; i++){
        int start = j + 1;
        if(i < remainder){
            j = j + factor + 1;
        }
        else{
            j = j + factor;
        }
        int end = j;

        //  Add values to struct
        args[i].num = i;
        args[i].start = start;
        args[i].end = end;

        //  Create threads
        pthread_create(&(tid[i]), NULL, SieveOfEratosthenes, &args[i]);
    }

    //  Join threads
    for(i = 0; i < threads; i++){
        pthread_join(tid[i], NULL);
    }

    //  Stop clock
    stop = clock();

    //  Calculate total time
    total = (double) (stop - start) / CLOCKS_PER_SEC;

    //  Print primes
    printf("Calculation Finished\n");
    printf("--------------------\n");
    printf("Primes found: ");
    for(i = 1; i < largest; i++){
        if(primes[i] == 0){
            printf("%d ", i + 1);
        }
    }
    printf("\n");
    printf("Total time for calculation: %6.3f (s)\n\n", total);

    //  Return and Clean
    free(primes);
    pthread_mutex_destroy(&lock);
    pthread_exit (NULL);
    return 0;
}