#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#include "../main.h"


// Seed Input
extern int A[NMAX];
// Subset
extern int B[NMAX];
// Output
extern int C[2*NMAX];

// Scratch area
extern int AA[NMAX / LOG2_NMAX];
extern int BB[NMAX / LOG2_NMAX];
extern int S[NMAX];

extern pthread_barrier_t barr, internal_barr;

void init(int n){
    /* Initialize the input for this iteration*/
    // B <- A
    int i;
    for(i = 0; i < n; i++)
    {
       B[i] = i+1; 
    }
    B[i] = i;
}

void seq_function(int n, int log_n){
    /* The code for sequential algorithm */
    // Perform operations on B

    int i, x;

    for(i = 0; i < n; i++)
    {
        S[i] = B[i];

        if(i != S[i])
            C[i] = 1;
        else
            C[i] = 0;
    }

    for(x = 0; x < log_n; x++)
    {
        for(i = 0; i < n; i++)
        {
            if(S[i] != S[S[i]])
            {
                int pos = S[i];
                C[i] = C[i] + C[pos];
                int next = S[pos];
                S[i] = next;
            }
        }
    }
}

void omp_function(int n, int log_n, int threads){
    /* The code for sequential algorithm */
    // Perform operations on B

    int i, x;

    for(i = 0; i < n; i++)
    {
        S[i] = B[i];

        if(i != S[i])
            C[i] = 1;
        else
            C[i] = 0;
    }

    for(x = 0; x < log_n; x++)
    {
        #pragma omp parallel for schedule(static, threads) shared(S,C) private(i) num_threads(threads)
        for(i = 0; i < n; i++)
        {
            if(S[i] != S[S[i]])
            {
                int pos = S[i];
                C[i] = C[i] + C[pos];
                int next = S[pos];
                S[i] = next;
            }
        }
    }
}

void par_function(int n, int log_n, int j, int start_n, int end_n){
    int i;

    for(i = start_n; i < end_n; i++)
    {
        /* The code for threaded computation */
        // Perform operations on B
    }
}
