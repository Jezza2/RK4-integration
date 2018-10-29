/*
CHAOS

Author:  Jeremy Stanger
Date:    22/09/2016

This file is basically just housekeeping; no interesting
computing going on here.

*/

#include <stdio.h>
#include <stdlib.h>
#include "header.h"


/* Write a gnuplot script to produce the required plot
	 Plot depends on programme execution mode.
	 Gnuplot can then be called externally

	style: style of the plot, i.e. which variables
	 */
void plot (int style) {
	// File pointer
	FILE *fp;
	// Character buffer for file name
	char filename[35] = { };

	// Set filename according to r and the plotting style
	sprintf(filename, "plots/plot_%g_%d.p", r, style);
	// Open file for writing if possible, otherwise quit
	// with error message
	if ((fp = fopen(filename, "w")) == NULL) {
		printf("Unable to open file for plotting\n");
		_exit(3);
	}

	fprintf(fp,
		"set terminal jpeg size 1000,750\n"
		"set output \"plot_%g_%d.jpg\"\n"
		
		"set autoscale\n"
		"set zeroaxis\n"
		"set xlabel \"y_1\"\n"
		"set ylabel \"y_2\"\n"
		"set zlabel \"y_3\"\n"
		"set xtic auto\n"
		"set ytic auto\n"
		"set ztic auto\n"
		"set key off\n"
		"set grid\n", r, style
	);

	switch (style) {
		case 0:   // 3d case
			fprintf(fp, "splot \"../data/data_%g.dat\" u 2:3:4 with lines", r);
			break;

		case 1:   // y_1 against t
			fprintf(fp, "set xlabel \"t\"\n"
									"set ylabel \"y_1\"\n"
									"plot \"../data/data_%g.dat\" u 1:2 with lines", r);
			break;

		case 2:   // y_2 against y_3
			fprintf(fp, "set xlabel \"y_3\"\n"
									"set ylabel \"y_2\"\n"
									"plot \"../data/data_%g.dat\" u 4:3 with lines", r);
			break;
	}

	// Close file
	fclose(fp);
}

/* Writes data in arrays to a file for plotting purposes
	 Space delimited file:
	 t y1 y2 y3
	 */
void write_datafile(void) {
	// File pointer
	FILE *fp;
	// index variables
	int i, j;
	// string buffer for file name
	char filename[35] = { };

	// Set file name according to the value of r
	sprintf(filename, "data/data_%g.dat", r);
	// Open file for writing if possible, otherwise quit
	// with error message	
	if ((fp = fopen(filename, "w")) == NULL) {
		printf("Unable to open file to write data\n");
		_exit(4);
	}

	for (i = 0; i < num_points; i++) {
		fprintf(fp, "%.9g ", (sol+i)->t);
		for (j = 0; j < NUM_DEP_VAR; j++) {
			fprintf(fp, "%.9g ", (sol+i)->y[j]);	
		}
		fprintf(fp, "\n");
	}
	// Close file
	fclose(fp);
}

/* Get user parameters and set global variables accordingly.
	 Also allocate memory required for the data storage.

	 count:  Number of parameters
	 *argvec[]: pointer to string array of the parameters
	 */
void set_params(int count, char *argvec[]) {

	// Test that we have the correct number of input parameters
	if (count != NUM_DEP_VAR + 6) {
		help();
		_exit(2);
	}

	// Assign some global variables
	if ((num_points = atoi(*(++argvec))) == 0 
		|| (delta_t = atof(*(++argvec))) == 0) {

		help();
		_exit(2);
	}

	// Allocate memory for data array
	if ((sol = malloc(sizeof(struct data_point) * num_points)) == NULL) {
		_exit(1);
	}

	int i;
	// Assign the initial conditions.
	for (i = 0; i < NUM_DEP_VAR; i++) {
		// If the return value is zero AND the input value is not 0, then
		// an input error has occurred.
		if ((sol->y[i] = (double)atof(*(++argvec))) == 0 && (*argvec)[0] != '0') {
			help();
			_exit(2);
		}
	}

	// Assign values of the various constants
	if ( ((a = (double)atof(*(++argvec))) == 0 && (*argvec)[0] != '0') 
		|| ((b = (double)atof(*(++argvec))) == 0 && (*argvec)[0] != '0')
		|| ((r = (double)atof(*(++argvec))) == 0 && (*argvec)[0] != '0') ) {

		help();
		_exit(2);
	}
}

/* Exit cleanly, freeing any dynamically allocated memory first.
	 Good practice.  Display exit status.

	 code: Exit status
	 */
void _exit(int code) {
	free(sol);
	printf("Exit status: %d\n", code);
	exit(code);
}

/* Display parameter list and modes
	 */
void help(void) {
	printf("\n%d arguments are required as follows\n\n"
				 "int n               The number of points to plot\n"
				 "double delta_t      Step size\n"
				 "double y_1,0        The initial value of y_1\n"
				 "double y_2,0        The initial value of y_2\n"
				 "double y_3,0        The initial value of y_3\n"
				 "double a            A constant\n"
				 "double b            A constant\n"
				 "double r            A constant\n\n\n",
				 NUM_DEP_VAR + 5);

	printf("EXIT STATUSES:\n\n"
				 "0 -                 Successfully executed\n"
				 "1 -                 Unable to allocate memory for data arrays\n"
				 "2 -                 Input error\n"
				 "3 -                 Unable to open file for plotting\n"
				 "4 -                 Unable to open file to write data\n\n\n");
}
