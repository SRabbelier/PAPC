#include <assert.h>
#include <stdio.h>

#include "PAPC_c.h"

#define dim_x 16
#define log_x 4
#define dim_log dim_x / log_x

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

void pointer_distance(int P[dim_x], int D[dim_x])
{
	int S[dim_x] = { 0 };
	int i, x;

	for(i = 0; i < dim_x; i++)
	{
		S[i] = P[i];

		if(i != S[i])
			D[i] = 1;
		else
			D[i] = 0;
	}

	for(x = 0; x < log_x; x++)
	{
		#pragma omp parallel for schedule(static, 4) shared(S,P) private(i) num_threads(4)
		for(i = 0; i < dim_x; i++)
		{		
			if(S[i] != S[S[i]])
			{
				int pos = S[i];
				D[i] = D[i] + D[pos];
				int next = S[pos];
				S[i] = next;
			}
		}
	}
}

void calculate_pointer_distance()
{
	int A[dim_x] = {13, 12, 4, 15, 10, 9, 8, 11, 8, 7, 6, 14, 3, 2, 1, 0};
	int B[dim_x] = { 0 };
	int eq;

	pointer_distance(A, B);

	int Bp[dim_x] = {6, 10, 4, 8, 3, 15, 1, 13, 0, 14, 2, 12, 9, 5, 11, 7};

	eq = array_cmp(B, Bp);
	assert(eq == 1);
}

int main(int argc, char * argv[])
{
	calculate_pointer_distance();
	printf("Win\n");
	return 0;
}
