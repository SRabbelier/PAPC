#include <assert.h>
#include <stdio.h>
#include <pthread.h>

extern void seq_function(int n, int log_n);

#define dim_x 16
#define log_x 4

int array_cmp(int L[dim_x], int R[dim_x])
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

int B[dim_x] = {13, 12, 4, 15, 10, 9, 8, 11, 8, 7, 6, 14, 3, 2, 1, 0};
int C[dim_x] = { 0 };
int S[dim_x] = { 0 };

pthread_barrier_t internal_barr;

void calculate_pointer_distance()
{
	int eq;

	seq_function(dim_x, log_x);

	int Cp[dim_x] = {6, 10, 4, 8, 3, 15, 1, 13, 0, 14, 2, 12, 9, 5, 11, 7};

	eq = array_cmp(C, Cp);
	assert(eq == 1);
}

int main(int argc, char * argv[])
{
	calculate_pointer_distance();
	printf("Win\n");
	return 0;
}
