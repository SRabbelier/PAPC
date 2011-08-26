#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#define dim_x 16
#define dim_2x 2*dim_x
#define log_x 4

extern void seq_function(int n, int log_n);

int array_cmp(int L[dim_2x], int R[dim_2x])
{
	int x;
	for(x = 0; x < dim_x; x++)
	{
		int Lxi = L[x];
		int Rxi = R[x];

		if(Lxi != Rxi)
			return 0;
	}
	return 1;
}

int B[dim_x] = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597};
int B2[dim_x] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
int C[dim_2x] = { 0 };

int AA[dim_x / log_x];
int BB[dim_x / log_x];

pthread_barrier_t internal_barr;

void calculate_merge()
{
	int eq;

	seq_function(dim_x, log_x);

	int Cp[dim_2x] = {1, 2, 2, 3, 4, 5, 6, 8, 8, 10, 12, 13, 14, 16, 18, 20, 21, 22, 24, 26, 28, 30, 32, 34, 55, 89, 144, 233, 377, 610, 987, 1597};

	eq = array_cmp(C, Cp);
	assert(eq == 1);
}

int main(int argc, char * argv[])
{
	calculate_merge();
	printf("Win\n");
	return 0;
}
