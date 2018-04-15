#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[])
{
	int    count;        /* Prime count */
	int    first;        /* Index of first multiple */
	int    i;
	int    index;        /* Index of current prime */
	char  *marked;       /* Marks for 2,...,'n' */
	int    n;            /* Sieving from 2, ..., 'n' */
	int    N;            /* Size of sieve and loop bounds */
	int    prime;        /* Current prime */
	int    threadnum;	/* Number of threads, only for parallelized code */

	if (argc != 3) {
		printf ("Command line: %s <threadnum> <n>\n", argv[0]);
		exit (1);
	}
	threadnum = atoi(argv[1]);
	n = atoi(argv[2]);
	N = n+1;
	omp_set_num_threads(threadnum);


	marked = (char *) malloc (N);  //alocate slots for numbers in range [0,n]
	if (marked == NULL) {
		printf ("Cannot allocate enough memory\n");
		exit (1);
	}

	for (i = 0; i < N; i++) marked[i] = 1;
	marked[0] = 0;
	marked[1] = 0; // not primes
	index = 2;
	prime = 2;
	do {
		first = 2 * prime;
#pragma omp parallel for
		for (i = first; i < N; i += prime) marked[i] = 0;
		while (!marked[++index]) ;
		prime = index;
	} while (prime * prime <= n);

	count = 0;
#pragma omp parallel for reduction(+:count)
	for (i = 0; i < N; i++)
		count += marked[i];
	printf ("There are %d primes less than or equal to %d\n\n", count, n);
	return 0;
}
