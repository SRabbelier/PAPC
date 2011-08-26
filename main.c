#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "main.h"

extern void seq_function(int n, int log_n);
extern void par_function(int n, int log_n, int start_n, int end_n);

int Ns[NSIZE] = {4096, 8192, 16384, 32768, 65536, 131072, 262144};
int Ls[NSIZE] = {12, 13, 14, 15, 16, 17, 18};

typedef struct __ThreadArg {
    int id;
    int nrT;
    int n;
    int log_n;
    int start_n;
    int end_n;
    int nt;
} tThreadArg;


pthread_t callThd[NUM_THREADS];
pthread_mutex_t mutexpm;
pthread_barrier_t barr, internal_barr;

// Seed Input
int A[NMAX];

// Subset
int B[NMAX];
int B2[NMAX];
int B_[LOG2_NMAX+1][NMAX]

// Output
int C[2*NMAX];
int C_[LOG2_NMAX+1][NMAX]

// Scratch area
int AA[NMAX / LOG2_NMAX];
int BB[NMAX / LOG2_NMAX];
int S[NMAX];
int S_[LOG2_NMAX+1][NMAX]


void* par_function_dispatch(void* a)
{
    int iter;
    tThreadArg* x = (tThreadArg*)a;

    for(iter = 0; iter < TIMES; iter++)
    {
        par_function(x->n, x->log_n, x->start_n, x->end_n);
        pthread_barrier_wait(&barr);
    }
}

int main (int argc, char *argv[])
{
    struct timeval startt, endt;
    int i, j, k, nt, t, n, log_n, c, dt;
    void *status;
    pthread_attr_t attr;
    tThreadArg x[NUM_THREADS];

    /* Generate a seed input */
    srand ( time(NULL) );
    for(k=0; k<NMAX; k++){
        A[k] = rand();
    }

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    printf("|  NSize | Iter | Seq      | Op01     | Op02     | Op04     | Op08     | Op16     | Th01     | Th02     | Th04     | Th08     | Par16    |\n");

    // for each input size
    for(c=0; c < MAX_NSIZE; c++){
        n = Ns[c];
        log_n = Ls[c];
        printf("| %6d | %4d |",n,TIMES);

        /* Run sequential algorithm */
        dt=0;
        gettimeofday (&startt, NULL);
        for (t=0; t<TIMES; t++) {
            init(n);
            seq_function(n, log_n);
        }
        gettimeofday (&endt, NULL);
        dt = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
        printf(" %ld.%06ld | ", dt/1000000, dt%1000000);

        for(nt=1; nt<NUM_THREADS; nt=nt<<1){
            /* Run sequential algorithm */
            dt=0;
            gettimeofday (&startt, NULL);
            for (t=0; t<TIMES; t++) {
                init(n);
                omp_function(n, log_n, nt);
            }
            gettimeofday (&endt, NULL);
            dt = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
            printf("%ld.%06ld | ", dt/1000000, dt%1000000);
        }

        /* Run threaded algorithm(s) */
        for(nt=1; nt<NUM_THREADS; nt=nt<<1){
            if(pthread_barrier_init(&barr, NULL, nt+1))
            {
                printf("Could not create a barrier\n");
                return -1;
            }
            if(pthread_barrier_init(&internal_barr, NULL, nt))
            {
                printf("Could not create a barrier\n");
                return -1;
            }

            dt = 0;
            for (j=1; j<=/*NUMTHRDS*/nt; j++)
            {
                x[j].id = j;
                x[j].nrT=nt; // number of threads in this round
                x[j].n=n;  //input size
                x[j].log_n=log_n;  // lg(input size)
                x[j].start_n= (j-1)*n/nt;  // start pos
                x[j].end_n= (j)*n/nt;  // end pos
                x[j].nt = nt; // number of threads
                pthread_create(&callThd[j-1], &attr, par_function_dispatch, (void *)&x[j]);
            }

            gettimeofday (&startt, NULL);
            for (t=0; t<TIMES; t++)
            {
                init(n);
                pthread_barrier_wait(&barr);
            }
            gettimeofday (&endt, NULL);

            /* Wait on the other threads */
            for(j=0; j</*NUMTHRDS*/nt; j++)
            {
                pthread_join(callThd[j], &status);
            }

            if (pthread_barrier_destroy(&barr)) {
                printf("Could not destroy the barrier\n");
                return -1;
            }
            if (pthread_barrier_destroy(&internal_barr)) {
                printf("Could not destroy the barrier\n");
                return -1;
            }
            dt = (endt.tv_sec*1000000+endt.tv_usec) - (startt.tv_sec*1000000+startt.tv_usec);
            printf("%ld.%06ld | ", dt/1000000, dt%1000000);
        }
        printf("\n");
    }
    pthread_exit(NULL);
}
