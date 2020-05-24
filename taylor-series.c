//  Question 1: Taylor Series    //
//  7 February 2020              //
//  Author: Anna DeVries         //

/*      Libraries                       */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*      Defs                            */
#define NTAYLOR 30

/*      Main                            */
int main( int argc, char *argv[] ){
    //  Local variables
    float x_1;
    float fx_1;
    float intermediate_1;
    float factorial_1 = 1;
    double x_2;
    double fx_2;
    double intermediate_2;
    double factorial_2 = 1;

    //  Check to ensure proper program execution
    if ( argc < 2 || argc > 3){
        printf("Usage: ./<filename> <initial value of x>\n");
        return 1;
    }

    //  Define local variables
    x_1 = atoi(argv[1]);
    x_2 = atoi(argv[1]);

    //  Compute as float
    fx_1 = x_1 + 1;
    for(int i = 1; i <= NTAYLOR; i++){
        intermediate_1 = pow(x_1,i);
        for(int j = 1; j <= i; j++){
            factorial_1 = factorial_1 * j;
        }
        intermediate_1 = intermediate_1 / factorial_1;
        fx_1 += intermediate_1;
    }

    //  Print off float values
    printf("Computed Float Value: %.20f    Initial x value: %f\n", fx_1, x_1);

    //  Compute as double
    fx_2 = x_2 + 1;
    for(int i = 1; i <= NTAYLOR; i++){
        intermediate_2 = pow(x_2,i);
        for(int j = 1; j <= i; j++){
            factorial_2 = factorial_2 * j;
        }
        intermediate_2 = intermediate_2 / factorial_2;
        fx_2 += intermediate_2;
    }

    //  Print off double values
    printf("Computed Double Value: %.60lf    Initial x value: %lf\n", fx_2, x_2);

    return 0;
}
