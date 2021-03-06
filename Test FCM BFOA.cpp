// Test FCM BFOA.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <sstream> //istringstream
#include <fstream> // ifstream
#include <string> 
#include <vector>

#define INF DBL_MAX
#define PI acos(-1.0)

#define dimension 450

#define S       30      /* population size */
#define Sr      S/2     /* number to split */
#define ss      0.3    /* step size */
#define N_ed    20       /* number of elimination-dispersal events */
#define N_re    10       /* number of reproduction steps */
#define N_ch    20    /* number of chemotactic steps */
#define N_sl    10      /* swim length */
#define p_ed    0.01    /* eliminate probability */
#define d_attr  5.0     /* depth of the attractant */
#define w_attr  5.0     /* width of the attractant signal */
#define h_rep   d_attr  /* height of the repellant effect */
#define w_rep   10.0    /* width of the repellant */


/* bacterium */
typedef struct Cell
{
	double vect[dimension];     /* position in search space */
	double cost;                /* objective function value */
	double fitness;             /* cost value and attractant and repellent effects */
	double health;              /* the health of bacterium */
	double step_size;           /* step in the search area */
} Cell;

Cell population[S];             /* population of bacteria */

double space[dimension][2];     /* the boundaries of the search space */
double rand_vect[dimension];    /* direction of movement after a tumble */
double delta[dimension];        /* used in the normalization of the rand_vect */

double best = INF;              /* the best solution found during the search */
int fe_count = 0;               /* number of objective function evaluations */

								/* functions */

								/* compute objective function */
void objective_function(Cell *x);

/* compute cell-to-cell attraction and repelling effects */
void interaction(Cell *x);

/* generate random number from a to b */
double random_number(double a, double b);

/* set the bounds values for search space */
void initialize_space(double a, double b);

/* distribute the population within the search space */
void initialize_population();

/* tumble current_cell, one step in a random direction */
void tumble_step(Cell *new_cell, Cell *current_cell);

/* swim step of current_cell in a rand_vect direction */
void swim_step(Cell *new_cell, Cell *current_cell);

/* function that compares two Cell objects by health value */
int compare(struct Cell *left, struct Cell *right);

/* tumble and swim each member in the population */
void chemotaxis();

/* split the bacteria */
void reproduction();

/* elimination and dispersal event */
void elimination_dispersal();

/* run an algorithm */
void optimization();

double TARGET = 1;      /*Setpoint*/
double TARGET2 = 0;     /*Setpoint*/
double TARGET3 = 0;     /*Setpoint*/

int main()
{
	srand(1);

	printf("Bacterial Foraging Optimization Algorithm\n");
	printf("Dimension: %d\n", dimension);

	/* search space [-100, 100]^dimension */
	initialize_space(-1, 1);
	/* random initialization within the search space */
	initialize_population();
	/* minimization of objective function */
	optimization();

	return 0;
}

const int FIL = 29, COL = 29;
double W[29][29];
double diffm[5][29];
double Wprint[29][29];
double lambdaprint;
double C[29];
double Lambda = 0;

void objective_function(Cell *x)
{
	double rez = 0.0;
	fe_count++;
	double output = 0.0, output1 = 0.0, output2 = 0.0;
	int lastvalue = 0, stopt = 0, yy = 0;
	double AUX[FIL], ITER[FIL], INPUT[FIL], DIF[FIL];
	double R[29];
	int full = 0;

	// Se convierte a Matriz 
	for (int i = 0; i < 29; i++) { for (int j = 0; j < 29; j++) { W[i][j] = 0; } }

	for (int i = 0; i < 29; i++) {
		for (int j = 0; j < 29; j++)
		{
			if ((j > 13)) { W[i][j] = x->vect[full]; full++; }
			if (i == j) { W[i][j] = 0; }
			if (i == 27) { W[27][j] = 0;} 
			if (i == 28) { W[28][j] = 0; }
			if (i == 29) { W[29][j] = 0; }
		}
	}

	Lambda = fabs((x->vect[full]) * 10);
	int MaxIter = 20;
	double suma = 0;

	for (int p = 0; p < 16; p++)
	{
		if (p == 0) { double C[] = { 0.1073,	0.0645,	0,	0,	0,	0.0474,	0.053,	0.0638,	0.0776,	0.0455,	0.0163,	0.0582,	0.2238,	0.2425, 0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 1) { double C[] = { 0.1033,	0.0054,	0,	0,	0,	0.0558,	0.0787,	0.1384,	0.0933,	0.0595,	0.0082,	0.1594,	0.1387,	0.1592,	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 2) { double C[] = { 0.1023,	0.0443,	0,	0,	0,	0.0636,	0.084,	0.114,	0.1219,	0.0596,	0.014,	0.1062,	0.0987,	0.1913,	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 3) { double C[] = { 0.2005,	0.0598,	0,	0,	0,	0.0925,	0.0807,	0.0808,	0.0668,	0.0425,	0.0099,	0.0693,	0.1588,	0.1382, 0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 4) { double C[] = { 0.045,	0,	0,	0,	0,	0.145,	0.15,	0.1383,	0.1105,	0.0411,	0.0235,	0.0397,	0.1761,	0.1308,	        0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 5) { double C[] = { 0.037,	0,	0,	0,	0,	0.1868,	0.1431,	0.0744,	0.0994,	0.0527,	0.038,	0.2175,	0.1055,	0.0455,	        0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 6) { double C[] = { 0.0393,	0.0118,	0,	0,	0,	0.0752,	0.0579,	0.0579,	0.037,	0.0222,	0.0095,	0,	0.3654,	0.3238,	    0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 7) { double C[] = { 0.0726,	0,	0,	0,	0,	0.2103,	0.109,	0.1223,	0.0786,	0.026,	0.0455,	0.1091,	0.1362,	0.0905,	    0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };  TARGET = 1; TARGET2 = 0; TARGET3 = 0; }
		if (p == 8) { double C[] = { 0.0513,	0,	0,	0,	0,	0.1383,	0.1701,	0.1194,	0.1043,	0.055,	0,	0.0871,	0.1424,	0.1322,	        0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }
		if (p == 9) { double C[] = { 0.2196,	0.2184,	0,	0,	0,	0.0386,	0.0559,	0.0681,	0.103,	0.0355,	0.0129,	0.0295,	0.1061,	0.1125, 0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }
		if (p == 10) { double C[] = { 0.0689,	0.0124,	0,	0,	0,	0.1108,	0.0762,	0.0819,	0.0774,	0.0176,	0.033,	0.0306,	0.2948,	0.1963,	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }
		if (p == 11) { double C[] = { 0.1011,	0.0269,	0,	0,	0,	0.2012,	0.0972,	0.0921,	0.0608,	0.0098,	0.0086,	0,	0.2864,	0.1158,	    0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }
		if (p == 12) { double C[] = { 0.0545,	0.0089,	0,	0,	0,	0.1344,	0.0604,	0.0856,	0.0588,	0.0233,	0.018,	0.0244,	0.3004,	0.2313,	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }
		if (p == 13) { double C[] = { 0.0867,	0.0337,	0,	0,	0,	0.089,	0.0873,	0.1081,	0.0934,	0.0471,	0.0531,	0.0372,	0.2194,	0.1451,	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }
		if (p == 14) { double C[] = { 0.1133,	0.0241,	0,	0,	0,	0.0841,	0.1286,	0.137,	0.0969,	0.0222,	0,	0,	0.1835,	0.2104,	        0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }
		if (p == 15) { double C[] = { 0.1243,	0.0221,	0,	0,	0,	0.1619,	0.1095,	0.0698,	0.0442,	0.0212,	0,	0,	0.2581,	0.1889,	        0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 }; TARGET = 1; TARGET2 = 1; TARGET3 = 0; }


		// Ecuacion FCM
		///////////////////////////////////////////////

		for (int iter = 0; iter <= MaxIter; iter++)
		{
			for (int i = 0; i < FIL; i++) {
				R[i] = 0;
				AUX[i] = 0;
				ITER[i] = 0;
				for (int j = 0; j < COL; j++) {
					AUX[i] += C[j] * W[j][i];
				}
				ITER[i] += 1 / (1 + (exp(-Lambda * AUX[i])));
				R[i] += ITER[i];
			}
			if (iter >= 15) {
				yy++;
				for (int jt = 0; jt < FIL; jt++) {
					diffm[yy][jt] = fabs(R[jt] - C[jt]);
				}


			}

		}
		suma = 0;
		for (int io = 0; io <= 5; io++)
		{
			for (int jo = 27; jo<FIL; jo++)
			{
				suma += (diffm[io][jo]);
			}
		}
		if (suma < 0.1) { stopt = 0; }
		else { stopt = 2; }

		for (int iio = 0; iio <= 5; iio++)
		{
			for (int jjo = 0; jjo<FIL; jjo++)
			{
				diffm[iio][jjo] = 0;
			}
		}

		for (int j = 0; j < FIL; j++) { C[j] = R[j]; lastvalue = j; };

		output1 = fabs(TARGET - R[lastvalue]);
		output = fabs(TARGET2 - R[lastvalue - 1]);
		output2 = fabs(TARGET3 - R[lastvalue - 2]);
		rez += (output1 + output + output2 + stopt);
	}


	//////////////////////////////////////////////////////////////////////
	//////////// store the best cell
	x->cost = rez;

	if (x->cost < best)
	{
		for (int ij = 0; ij < FIL; ij++) {
			for (int jk = 0; jk < FIL; jk++)
			{
				Wprint[ij][jk] = 0;
			}
		}
		best = x->cost;
		lambdaprint = Lambda;
		for (int i = 0; i < FIL; i++) {
			for (int j = 0; j < FIL; j++)
			{
				Wprint[i][j] = W[i][j];
			}
		}
	}
}

double random_number(double a, double b)
{
	return ((((double)rand()) / ((double)RAND_MAX)) * (b - a) + a);
}
void initialize_space(double a, double b)
{
	int i;
	for (i = 0; i < dimension; i++)
	{
		space[i][0] = a;
		space[i][1] = b;
	}
}
int compare(struct Cell *left, struct Cell *right)
{
	if (left->health < right->health)
		return -1;
	if (left->health > right->health)
		return 1;
	return 0;
}
void initialize_population()
{
	/* randomly distribute the initial population */
	int i, j;
	for (i = 0; i < S; i++)
	{
		for (j = 0; j < dimension; j++)
		{
			population[i].vect[j] = random_number(space[j][0], space[j][1]);
		}
		objective_function(&population[i]);
		population[i].fitness = 0.0;
		population[i].health = 0.0;
		population[i].step_size = ss;
	}
}
void elimination_dispersal()
{
	int i, j;
	for (i = 0; i < S; i++)
	{
		/* simply disperse bacterium to a random location on the search space */
		if (random_number(0.0, 1.0) < p_ed)
		{
			for (j = 0; j < dimension; j++)
			{
				population[i].vect[j] = random_number(space[j][0], space[j][1]);
			}
			objective_function(&population[i]);
		}
	}
}
void reproduction()
{
	/* sort the population in order of increasing health value */
	qsort(population, S, sizeof(Cell), (int(*)(const void*, const void*))compare);
	int i, j;
	/* Sr healthiest bacteria split into two bacteria, which are placed at the same location */
	for (i = S - Sr, j = 0; j < Sr; i++, j++)
	{
		population[i] = population[j];
	}
	for (i = 0; i < S; i++)
	{
		population[i].health = 0.0;
	}
}
void interaction(Cell *x)
{
	int i, j;
	double attract = 0.0, repel = 0.0, diff = 0.0;
	for (i = 0; i < S; i++)
	{
		diff = 0.0;
		for (j = 0; j < dimension; j++)
		{
			diff += pow(x->vect[j] - population[i].vect[j], 2.0);
		}
		attract += -1.0*d_attr*exp(-1.0*w_attr*diff);
		repel += h_rep * exp(-1.0*w_rep*diff);
	}
	/* this produces the swarming effect */
	x->fitness = x->cost + attract + repel;
}
void tumble_step(Cell *new_cell, Cell *current_cell)
{
	int i;
	double a = -1.0, b = 1.0, temp1 = 0.0, temp2 = 0.0;
	for (i = 0; i < dimension; i++)
	{
		delta[i] = random_number(a, b);
		temp1 += pow(delta[i], 2.0);
	}
	temp2 = sqrt(temp1);
	for (i = 0; i < dimension; i++)
	{
		rand_vect[i] = delta[i] / temp2;
		new_cell->vect[i] = current_cell->vect[i] + current_cell->step_size*rand_vect[i];
		/* there is no need to perform search outside of the given bounds */
		if (new_cell->vect[i] < space[i][0])
			new_cell->vect[i] = space[i][0];
		if (new_cell->vect[i] > space[i][1])
			new_cell->vect[i] = space[i][1];
	}
}
void swim_step(Cell *new_cell, Cell *current_cell)
{
	int i;
	for (i = 0; i < dimension; i++)
	{
		new_cell->vect[i] = new_cell->vect[i] + current_cell->step_size*rand_vect[i];
		/* there is no need to perform search outside of the given bounds */
		if (new_cell->vect[i] < space[i][0])
			new_cell->vect[i] = space[i][0];
		if (new_cell->vect[i] > space[i][1])
			new_cell->vect[i] = space[i][1];
	}
}
void chemotaxis()
{
	double Jlast;
	Cell new_cell;
	int i, j, m;
	for (i = 0; i < S; i++)
	{
		interaction(&population[i]);
		Jlast = population[i].fitness;
		tumble_step(&new_cell, &population[i]);
		objective_function(&new_cell);
		interaction(&new_cell);
		for (j = 0; j < dimension; j++)
		{
			population[i].vect[j] = new_cell.vect[j];
			population[i].cost = new_cell.cost;
			population[i].fitness = new_cell.fitness;
			population[i].health += population[i].fitness;
		}

		for (m = 0; m < N_sl; m++)
		{
			if (new_cell.fitness < Jlast)
			{
				Jlast = new_cell.fitness;
				swim_step(&new_cell, &population[i]);
				objective_function(&new_cell);
				interaction(&new_cell);
				for (j = 0; j < dimension; j++)
				{
					population[i].vect[j] = new_cell.vect[j];
					population[i].cost = new_cell.cost;
					population[i].fitness = new_cell.fitness;
					population[i].health += population[i].fitness;
				}

			}
			else break;
		}
	}

}
void optimization()
{
	int l, k, j;
	for (l = 0; l < N_ed; l++)           /* Elimination-dispersal loop */
	{
		for (k = 0; k < N_re; k++)       /* Reproduction loop */
		{
			for (j = 0; j < N_ch; j++)   /* Chemotaxis loop */
			{
				chemotaxis();
				printf("best=%e , fe_count=%d\n", best, fe_count);

			}
			reproduction();
		}
		elimination_dispersal();
	}

	printf("\nbest found value: %e, number of function evaluations: %d\n", best, fe_count);
	printf(" \n");

	for (int i = 0; i < 29; i++) {
		for (int j = 0; j < 29; j++)
		{
			printf("%f ", Wprint[i][j]);
		}
		printf(" \n");
	}
	printf("%f ", lambdaprint);
}
