#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "../main.h"


// ld_i = log_n
// dim_i = n
// dim_x = log_n + 1

// B = B_
// S = S_
// P = C_

// Subset
extern int B[NMAX];
extern int B_[LOG2_NMAX+1][NMAX];

// Output
extern int C[2*NMAX];
extern int C_[LOG2_NMAX+1][NMAX];

// Scratch area
extern int S_[LOG2_NMAX+1][NMAX];

extern pthread_barrier_t internal_barr;

void init(int n){
    /* Initialize the input for this iteration*/
    // B <- A
    int i;
    for(i = 0; i < n; i++)
    {
		B[i] = i;
    }
}

void seq_function(int n, int log_n)
{
    int i, j, h;

	for(j = 0; j < n; j++)
	{
		B_[0][j] = B[j];
	}

	for(h = 1; h <= log_n; h++)
	{
		int h_pow = pow(2, h);
		int size = n / h_pow;
		for(j = 1; j <= size; j++)
		{
			int prev, curr, next;

			prev = B_[h-1][2*j-2];
			curr = B_[h-1][2*j-1];
			next = prev < curr ? prev : curr;
			B_[h][j-1] = next;
		}

	}

	int x;
	int prev_size = 0;
	int size = 1;

	for(x = log_n; x >= 0; x--)
	{
		for(i = 0; i < size; i++) 
		{
			int curr, prev_pos, prev, next;

			curr = B_[x][i];

			prev_pos = ceil((float)i/2.0)-1;

			if(prev_pos < 0)
			{
				next = curr;
			}
			else
			{
				prev = C_[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}

			C_[x][i] = next;

			prev_pos = ceil((float)i/2.0);

			if(prev_pos >= prev_size)
			{
				next = curr;
			}
			else
			{
				prev = S_[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}
			S_[x][i] = next;
		}

		prev_size = size;
		size *= 2;
	}

    for(i = 0; i < n; i++)
    {
        C[i] = C_[0][i];
    }
}

void omp_function(int n, int log_n, int threads)
{
    int i, j, h;

	for(j = 0; j < n; j++)
	{
		B_[0][j] = B[j];
	}

	for(h = 1; h <= log_n; h++)
	{
		int h_pow = pow(2, h);
		int size = n / h_pow;
		#pragma omp parallel for schedule(static, threads) shared(B_,h_pow,size,h) private(j) num_threads(threads)
		for(j = 1; j <= size; j++)
		{
			int prev, curr, next;

			prev = B_[h-1][2*j-2];
			curr = B_[h-1][2*j-1];
			next = prev < curr ? prev : curr;
			B_[h][j-1] = next;
		}

	}

	int x;
	int prev_size = 0;
	int size = 1;

	for(x = log_n; x >= 0; x--)
	{
		#pragma omp parallel for schedule(static, threads) shared(B_,C_,S_,x,prev_size,size) private(i) num_threads(threads)
		for(i = 0; i < size; i++)
		{
			int curr, prev_pos, prev, next;

			curr = B_[x][i];

			prev_pos = ceil((float)i/2.0)-1;

			if(prev_pos < 0)
			{
				next = curr;
			}
			else
			{
				prev = C_[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}

			C_[x][i] = next;

			prev_pos = ceil((float)i/2.0);

			if(prev_pos >= prev_size)
			{
				next = curr;
			}
			else
			{
				prev = S_[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}
			S_[x][i] = next;
		}

		prev_size = size;
		size *= 2;
	}

    for(i = 0; i < n; i++)
    {
        C[i] = C_[0][i];
    }
}

void par_function(int n, int log_n, int id, int nt)
{
    int i, j, h;

    int start_n= (id-1)*n/nt;  // start pos
    int end_n= (id)*n/nt;  // end pos

	for(j = start_n; j < end_n; j++)
	{
		B_[0][j] = B[j];
	}

    pthread_barrier_wait(&internal_barr);

	for(h = 1; h <= log_n; h++)
	{
		int h_pow = pow(2, h);
		int size = n / h_pow;
        int start = (id-1)*size/nt + 1;  // start pos
        int end= (id)*size/nt;  // end pos
		for(j = start; j <= end; j++)
		{
			int prev, curr, next;

			prev = B_[h-1][2*j-2];
			curr = B_[h-1][2*j-1];
			next = prev < curr ? prev : curr;
			B_[h][j-1] = next;
		}

	}

    pthread_barrier_wait(&internal_barr);

	int x;
	int prev_size = 0;
	int size = 1;

	for(x = log_n; x >= 0; x--)
	{
        int start = (id-1)*size/nt;  // start pos
        int end= (id)*size/nt;  // end pos
		for(i = start; i < end; i++)
		{
			int curr, prev_pos, prev, next;

			curr = B_[x][i];

			prev_pos = ceil((float)i/2.0)-1;

			if(prev_pos < 0)
			{
				next = curr;
			}
			else
			{
				prev = C_[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}

			C_[x][i] = next;

			prev_pos = ceil((float)i/2.0);

			if(prev_pos >= prev_size)
			{
				next = curr;
			}
			else
			{
				prev = S_[x+1][prev_pos];
				next = curr < prev ? curr : prev;
			}
			S_[x][i] = next;
		}

		prev_size = size;
		size *= 2;
        pthread_barrier_wait(&internal_barr);
	}

    for(i = start_n; i < end_n; i++)
    {
        C[i] = C_[0][i];
    }

    pthread_barrier_wait(&internal_barr);
}
