/*
CHAOS

Author:  Jeremy Stanger
Date:    22/09/2016

Header file.
Function prototypes, macros and external variable declarations

*/

// Some definitions and macros
// The number of independent variables
#define NUM_DEP_VAR 3

// Function prototypes

// Functions in io.c
/* Display parameter list and modes
	 */
void help(void);   

/* Exit cleanly, freeing any dynamically allocated memory first.
	 Good practice.  Display exit status.

	 code: Exit status
	 */
void _exit(int code);

/* Get user parameters and set global variables accordingly.
	 Also allocate memory required for the data storage.

	 count:  Number of parameters
	 *argvec[]: pointer to string array of the parameters
	 */
void set_params(int count, char *argvec[]);

/* Writes data in arrays to a file for plotting purposes
	 Space delimited file:
	 t y1 y2 y3
	 */
void write_datafile(void);

/* Write a gnuplot script to produce the required plot
	 Plot depends on programme execution mode.
	 Gnuplot can then be called externally
	 */
void plot(int order);


// Functions in chaos.c
/* Populate sol with solution using the RK4 method 
	 */
void calc_sol(void);

/* Return f_i(y1, y2, y3)

	i : index of function to execute
	y : Dependent variables
	 */
double func(int i, double y1, double y2, double y3);

// Declare some global variables
// Input parameters
// a constant
extern double a;
// a constant
extern double b;
// Difference between 2 successive values of t
extern double delta_t;
// a constant
extern double r;
// Number of points in plot
extern int num_points;


// Data storage arrays
struct data_point {
	double t;
	double y[NUM_DEP_VAR];
} *sol;
