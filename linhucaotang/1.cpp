#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int count = 0;
void solve(int n, int col, int *lhist)
{
	if (col == n)
	{
#pragma omp atomic
		++count;
		return;
	}

# define attack(i, j) (lhist[j] == i || abs(lhist[j] - i) == col - j)
	for (int i = 0, j = 0; i < n; i++)
	{
		for (j = 0; j < col && !attack(i, j); j++);
		if (j < col) continue;

		int hist[n];
		for(int k=0;k<n;k++) hist[k]=lhist[k];
		hist[col] = i;
#pragma omp task
		solve(n, col + 1, hist);
	}
}

int main(int n, char **argv)
{
	if (n < 2) exit(1);
	int threadnum = atoi(argv[1]);
	omp_set_num_threads(threadnum);
	if (n == 2 || (n = atoi(argv[2])) <= 0) n = 8;
	int hist[n];
#pragma omp parallel
	{
#pragma omp single
		solve(n, 0, hist);
	}
	printf("%d solutions\n", count);
}
