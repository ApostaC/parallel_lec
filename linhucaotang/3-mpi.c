#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mpi.h"
#ifndef LOCAL
#include "fio.h"
#else
ssize_t output_data(double * pi,ssize_t a)
{
	fprintf(stdout,"pi = %lf\n",*pi);
}
#endif

#define SQ(a)	((a)*(a))
#define CAL(i)	\
	(4/(1+SQ((i-0.5)/n)))

int main(int argc, char *argv[])
{
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);  
	MPI_Comm_size(MPI_COMM_WORLD,&size);  

	uint64_t n = atoi(argv[1]);
	n = (1ull<<n);
	double sum;
	for(uint64_t i=rank;i<n;i+=size)
		sum+=CAL(i);

	double ans = 0.0;

//	fprintf(stderr,"%d : %ld\n",rank,n);
	/* reduce */
	MPI_Reduce(&sum,&ans,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

	if(rank == 0)
	{
		ans /= n;
		output_data(&ans,sizeof ans);
	}
	MPI_Finalize();
}
