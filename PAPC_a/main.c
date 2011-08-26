#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#include "../main.h"

extern void seq_function(int n, int log_n);

#define log_x 3
#define log_x_1 4
#define dim_x 8

/**
 *
 * Compares two dim_x*dim_i arrays.
 *
 * Returns 1 if equal, 0 if not equal.
 *
 **/
int array_cmp(int L[LOG2_NMAX+1][NMAX], int R[log_x_1][dim_x])
{
	int i, x;
	for(x = 0; x < log_x_1; x++)
	{
		for(i = 0; i < dim_x; i++)
		{
			int Lxi = L[x][i];
			int Rxi = R[x][i];

			if(Lxi != Rxi)
				return 0;
		}
	}
	return 1;
}

int B[dim_x] = {6, 9, 5, 8, 2, 3, 4, 1};
int B_[LOG2_NMAX+1][NMAX] = { 0 };

int S_[LOG2_NMAX+1][NMAX] = { 0 };

int C[dim_x] = { 0 };
int C_[LOG2_NMAX+1][NMAX] = { 0 };

int calculate_prefix_minima()
{
	int eq;

	int Cp[log_x_1][dim_x] = {
		{6, 6, 5, 5, 2, 2, 2, 1},
		{6, 5, 2, 1},
		{5, 1},
		{1},
	};

	seq_function(dim_x, log_x);

	eq = array_cmp(C_, Cp);
	assert(eq == 1);
}

int main(int argc, char * argv[])
{
	calculate_prefix_minima();
	printf("Win\n");
	return 0;
}
