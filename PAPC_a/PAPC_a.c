#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "PAPC_a.h"

#define dim_x 4
#define dim_i 8
#define ld_i 3

/**
  A = [6,9,5,8,2,3,4,1]
  B =
0: 6, 9, 5, 8, 2, 3, 4, 1
1: 6, 5, 2, 1
2: 5, 1
3: 1
 */ 

void prefix(const int A[dim_x], int B[dim_x][dim_i])
{
	int j, h;

	for(j = 0; j < dim_i; j++)
	{
		B[0][j] = A[j];
	}

	for(h = 1; h <= ld_i; h++)
	{
		// B[h,j] := B[h - 1, 2j -1] + B[h - 1, 2j]
		int h_pow = pow(2, h);
		int size = dim_i / h_pow;
		for(j = 1; j <= size; j++)
		{
			int prev, curr, next;

			prev = B[h-1][2*j-2];
			curr = B[h-1][2*j-1];
			next = prev < curr ? prev : curr;
			B[h][j-1] = next;
		}

	}
}

void minima(int B[dim_x][dim_i], int P[dim_x][dim_i], int S[dim_x][dim_i])
{
	int x, i;
	int prev_size = 0;
	int size = 1;

	for(x = dim_x-1; x >= 0; x--)
	{
		for(i = 0; i < size; i++) 
		{
			int curr, prev_pos, prev, next;

			curr = B[x][i];

			prev_pos = ceil((float)i/2.0)-1;

			if(prev_pos < 0)
			{
				next = curr;
			}
			else
			{
				prev = P[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}

			P[x][i] = next;

			prev_pos = ceil((float)i/2.0);

			if(prev_pos >= prev_size)
			{
				next = curr;
			}
			else
			{
				prev = S[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}
			S[x][i] = next;
		}

		prev_size = size;
		size *= 2;
	}
}

/**
 *
 * Compares two dim_x*dim_i arrays.
 *
 * Returns 1 if equal, 0 if not equal.
 *
 **/
int array_cmp(int L[dim_x][dim_i], int R[dim_x][dim_i])
{
	int i, x;
	for(x = 0; x < dim_x; x++)
	{
		for(i = 0; i < dim_i; i++)
		{
			int Lxi = L[x][i];
			int Rxi = R[x][i];

			if(Lxi != Rxi)
				return 0;
		}
	}
	return 1;
}

int calculate_prefix_minima()
{
	int A[dim_i] = {6, 9, 5, 8, 2, 3, 4, 1};
	int B[dim_x][dim_i] = { 0 };
	int eq;

	prefix(A,B);

	int Bp[dim_x][dim_i] = {
		{6, 9, 5, 8, 2, 3, 4, 1},
		{6, 5, 2, 1},
		{5, 1},
		{1},
	};

	eq = array_cmp(B, Bp);
	assert(eq == 1);

	int S[dim_x][dim_i] = { 0 };
	int P[dim_x][dim_i] = { 0 };
	int Pp[dim_x][dim_i] = {
		{6, 6, 5, 5, 2, 2, 2, 1},
		{6, 5, 2, 1},
		{5, 1},
		{1},
	};

	minima(B, P, S);

	eq = array_cmp(P, Pp);
	assert(eq == 1);
}

int calculate_prefix_minima_edge()
{
	int A[dim_i] = {1, 2, 2, 2, 2, 2, 2, 2};
	int B[dim_x][dim_i] = { 0 };
	int eq;

	prefix(A,B);

	int Bp[dim_x][dim_i] = {
		{1, 2, 2, 2, 2, 2, 2, 2},
		{1, 2, 2, 2},
		{1, 2},
		{1},
	};

	eq = array_cmp(B, Bp);
	assert(eq == 1);

	int S[dim_x][dim_i] = { 0 };
	int P[dim_x][dim_i] = { 0 };
	int Pp[dim_x][dim_i] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1},
		{1},
	};

	minima(B, P, S);

	eq = array_cmp(P, Pp);
	assert(eq == 1);
}

int calculate_suffix_minima()
{
	int A[dim_i] = {1, 4, 2, 3, 8, 6, 5, 9};
	int B[dim_x][dim_i] = { 0 };
	int eq;

	prefix(A,B);

	int Bp[dim_x][dim_i] = {
		{1, 4, 2, 3, 8, 6, 5, 9},
		{1,	2, 6, 5},
		{1, 5},
		{1},
	};

	eq = array_cmp(B, Bp);
	assert(eq == 1);

	int P[dim_x][dim_i] = { 0 };
	int S[dim_x][dim_i] = { 0 };
	int Sp[dim_x][dim_i] = {
		{1, 2, 2, 3, 5, 5, 5, 9},
		{1, 2, 5, 5},
		{1, 5},
		{1},
	};

	minima(B, P, S);

	eq = array_cmp(S, Sp);
	assert(eq == 1);
}

int calculate_suffix_minima_edge()
{
	int A[dim_i] = {2, 2, 2, 2, 2, 2, 2, 1};
	int B[dim_x][dim_i] = { 0 };
	int eq;

	prefix(A,B);

	int Bp[dim_x][dim_i] = {
		{2, 2, 2, 2, 2, 2, 2, 1},
		{2, 2, 2, 1},
		{2, 1},
		{1},
	};

	eq = array_cmp(B, Bp);
	assert(eq == 1);

	int P[dim_x][dim_i] = { 0 };
	int S[dim_x][dim_i] = { 0 };
	int Sp[dim_x][dim_i] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1},
		{1},
	};

	minima(B, P, S);

	eq = array_cmp(S, Sp);
	assert(eq == 1);
}

int main(int argc, char * argv[])
{
	calculate_prefix_minima();
	calculate_suffix_minima();
	calculate_prefix_minima_edge();
	calculate_suffix_minima_edge();
	printf("Win\n");
	return 0;
}
