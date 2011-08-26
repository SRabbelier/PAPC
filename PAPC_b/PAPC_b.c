#include <math.h>
#include <stdio.h>
#include <pthread.h>

#include "../main.h"


// Subset
extern int B[NMAX];
extern int B2[NMAX];
// Output
extern int C[2*NMAX];

// Scratch area
extern int AA[NMAX / LOG2_NMAX];
extern int BB[NMAX / LOG2_NMAX];
extern int S[NMAX];

extern pthread_barrier_t internal_barr;

void init(int n){
    /* Initialize the input for this iteration*/
    // B <- A
    int i;
    for(i = 0; i < n; i++)
    {
		B[i] = i+1;
		B2[i] = i*2;
    }
}

// adapted from http://en.wikipedia.org/wiki/Binary_search#Iterative
int rank(int value, int* A, int n)
{
	int low = 0;
	int high = n-1;
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

void seq_merge(int n, int start_a, int start_b)
{
	int finger_a = start_a;
	int finger_b = start_b;
	int next = finger_a + finger_b;

	do
	{
		int val_a = B[finger_a];
		int val_b = B2[finger_b];

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
	while((next < 2*n) && !C[next]);
}


void seq_function(int n, int log_n)
{
	int i;

	for(i = 0; i < n/log_n; i++)
	{
		int pos = i*log_n;

		int val_a = B[pos] - 1;
		AA[i] = rank(val_a, B2, n);
		int rank_a = AA[i] + pos;
		C[rank_a] = B[pos];

		int val_b = B2[pos];
		BB[i] = rank(val_b, B, n);
		int rank_b = BB[i] + pos;
		C[rank_b] = B2[pos];
	}

	for(i = 0; i < n/log_n; i++)
	{
		int pos = i*log_n;
		int rank = AA[i];
		seq_merge(n, pos, rank);

		rank = BB[i];
		seq_merge(n, rank, pos);
	}
}

void omp_function(int n, int log_n, int threads){
	int i;

	#pragma omp parallel for schedule(static, threads) shared(B,B2,C,AA,BB) private(i) num_threads(threads)
	for(i = 0; i < n/log_n; i++)
	{
		int pos = i*log_n;

		int val_a = B[pos] - 1;
		AA[i] = rank(val_a, B2, n);
		int rank_a = AA[i] + pos;
		C[rank_a] = B[pos];

		int val_b = B2[pos];
		BB[i] = rank(val_b, B, n);
		int rank_b = BB[i] + pos;
		C[rank_b] = B2[pos];
	}

	#pragma omp parallel for schedule(static, threads) shared(B,B2,C,AA,BB) private(i) num_threads(threads)
	for(i = 0; i < n/log_n; i++)
	{
		int pos = i*log_n;
		int rank = AA[i];
		seq_merge(n, pos, rank);

		rank = BB[i];
		seq_merge(n, rank, pos);
	}
}

void par_function(int n, int log_n, int id, int nt) {
	int i;

    int start_n= (id-1)*n/nt;  // start pos
    int end_n= (id)*n/nt;  // end pos

	for(i = start_n; i < end_n/log_n; i++)
	{
		int pos = i*log_n;

		int val_a = B[pos] - 1;
		AA[i] = rank(val_a, B2, n);
		int rank_a = AA[i] + pos;
		C[rank_a] = B[pos];

		int val_b = B2[pos];
		BB[i] = rank(val_b, B, n);
		int rank_b = BB[i] + pos;
		C[rank_b] = B2[pos];
	}

    pthread_barrier_wait(&internal_barr);

	for(i = start_n; i < end_n/log_n; i++)
	{
		int pos = i*log_n;
		int rank = AA[i];
		seq_merge(n, pos, rank);

		rank = BB[i];
		seq_merge(n, rank, pos);
	}

    pthread_barrier_wait(&internal_barr);
}
