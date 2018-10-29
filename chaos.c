/*
CHAOS

Author:  Jeremy Stanger
Date: 	 22/09/2016

Main file; where the business happens.

This programme will use the RK4 method to find solutions
to Lorenz's equations.
*/


#include <stdio.h>
#include <stdlib.h>
#include "header.h"


// Define some global variables
// Input parameters
// a constant
double a;
// a constant
double b;
// Difference between 2 successive values of t
double delta_t;
// a constant
double r;
// Number of points in plot
int num_points;

int main(int argc, char *argv[]) {

	set_params(argc, argv);

	calc_sol();
	write_datafile();
	// Plot y_1 against y_2 against y_3 on the x, y, z axes respectively.
	plot(0);
	// Plot y_1 against t
	plot(1);
	// Plot y_2 against y_3
	plot(2);

	printf("Successfully executed!\n");
	_exit(0);
}

/* Populate sol with solution using the RK4 method 
	 */
void calc_sol(void) {
	int i, j;
	// 'f's calculated at each stage, saved for use on completion of the step.	
	double currF[4][NUM_DEP_VAR];
	// sub interval width at each stage
	double int_delta;

	// Loop through all values of t
	// Each loop executes the respective stage of the RK4 method
	for (i = 0; i < num_points-1; i++) {
		(sol + i)->t = ((double)i)*delta_t;

		for (j = 0; j < NUM_DEP_VAR; j++) {
			currF[0][j] = func(j, (sol+i)->y[0], 
						 		(sol+i)->y[1], 
						 		(sol+i)->y[2]);
		}

		for (j = 0; j < NUM_DEP_VAR; j++) {
			int_delta = currF[0][j] * delta_t / 2.0;
			currF[1][j] = func(j, (sol+i)->y[0] + int_delta,
						 		(sol+i)->y[1] + int_delta, 
						 		(sol+i)->y[2] + int_delta);
		}

		for (j = 0; j < NUM_DEP_VAR; j++) {
			int_delta = currF[1][j] * delta_t / 2.0;
			currF[2][j] = func(j, (sol+i)->y[0] + int_delta,
						 		(sol+i)->y[1] + int_delta, 
						 		(sol+i)->y[2] + int_delta);
		}

		for (j = 0; j < NUM_DEP_VAR; j++) {
			int_delta = currF[2][j] * delta_t;
			currF[3][j] = func(j, (sol+i)->y[0] + int_delta,
						 		(sol+i)->y[1] + int_delta, 
						 		(sol+i)->y[2] + int_delta);
		}

		for (j = 0; j < NUM_DEP_VAR; j++) {
			(sol+i+1)->y[j] = (sol+i)->y[j] + (delta_t/6.0) * 
								(currF[0][j] + 2.0*currF[1][j] + 
								2.0*currF[2][j] + currF[3][j]);
		}
	}
	(sol+num_points-1)->t = ((double)(num_points-1))*delta_t;
}

/* Return f_i(y1, y2, y3)

	i : index of function to execute
	y : Dependent variables
	 */
double func(int i, double y1, double y2, double y3) {
	switch (i) {
		case 0:
			return a * (y2 - y1);
		case 1:
			return r * y1 - y2 - y1 * y3;
		case 2:
			return y1 * y2 - b * y3;
	}
	return 0.0;
}
