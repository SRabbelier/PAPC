#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "PAPC_b.h"

#define dim_x 16
#define dim_2x 2*dim_x
#define log_x 4
#define dim_log dim_x / log_x

// adapted from http://en.wikipedia.org/wiki/Binary_search#Iterative
int rank(int value, int A[dim_x])
{
	int low = 0;
	int high = dim_x-1;
	int mid;

	while (low <= high)
	{
		mid = low + ((high - low) / 2);

		if (A[mid] > value) {
			high = mid - 1;
		} else if (A[mid] < value) {
			low = mid + 1;
		} else {
			return mid;
		}
	}

	return low;
}

void seq_merge(int A[dim_x], int B[dim_x], int C[dim_2x], int start_a, int start_b)
{
	int finger_a = start_a;
	int finger_b = start_b;
	int next = finger_a + finger_b;

	do
	{
		int val_a = A[finger_a];
		int val_b = B[finger_b];

		if(val_a < val_b)
		{
			C[next] = val_a;
			finger_a++;
		}
		else
		{
			C[next] = val_b;
			finger_b++;
		}
		next++;
	}
	while((next < dim_2x) && !C[next]);
}

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

void merge(int A[dim_x], int B[dim_x], int C[dim_2x])
{
	int AA[log_x] = { 0 };
	int BB[log_x] = { 0 };
	int i;

	#pragma omp parallel for schedule(static, 1) shared(A,B,C,AA,BB) private(i) num_threads(4)
	for(i = 0; i < dim_x/log_x; i++)
	{
		int pos = i*log_x;

		int val_a = A[pos] - 1;
		AA[i] = rank(val_a, B);
		int rank_a = AA[i] + pos;
		C[rank_a] = A[pos];

		int val_b = B[pos];
		BB[i] = rank(val_b, A);
		int rank_b = BB[i] + pos;
		C[rank_b] = B[pos];
	}

	#pragma omp parallel for schedule(static, 1) shared(A,B,C,AA,BB) private(i) num_threads(4)
	for(i = 0; i < dim_x/log_x; i++)
	{
		int pos = i*log_x;
		int rank = AA[i];
		seq_merge(A, B, C, pos, rank);

		rank = BB[i];
		seq_merge(A, B, C, rank, pos);
	}

}

void calculate_merge()
{
	int A[dim_x] = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597};
	int B[dim_x] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
	int C[dim_2x] = { 0 };
	int eq;

	merge(A, B, C);

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
