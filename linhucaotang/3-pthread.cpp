#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <algorithm>
#include <cstdlib>
#include <stdint.h>
#ifndef LOCAL
#include "fio.h"
#endif

using namespace std;
#ifdef LOCAL
ssize_t output_data(void* buf, ssize_t count);
#endif

uint64_t n;
int p;
double ans;
pthread_mutex_t lock;
#define SQ(a)	((a)*(a))
#define CAL(i)	\
	(4/(1+SQ((i-0.5)/n)))
void * worker(void * a)
{
	uint64_t stride = p;
	uint64_t start = (uint64_t)(a);
	double sum = 0.0;
	for(uint64_t i = start;i<n;i+=stride)
	{
		sum+=CAL(i);
	}

	pthread_mutex_lock(&lock);
	ans += sum;
	pthread_mutex_unlock(&lock);

}

int main(int argc, char *argv[])
{
	pthread_mutex_init (&lock, NULL); 
	if(argc<3)
	{
		double pi = 3.1415926535;
		output_data(&pi,sizeof pi);
		return 0;
	}
	p = atoi(argv[1]);
	n = atoi(argv[2]);
	n = (1ull<<n);

	pthread_t workers[p];
	for(uint64_t i=0;i<p;i++)
		pthread_create(workers+i,NULL,worker,(void *)i);
	for(int i=0;i<p;i++)
		pthread_join(workers[i],NULL);
	ans/=n;
	output_data(&ans,sizeof(ans));
	return 0;
}

#ifdef LOCAL
#define _ELEM_T		double
ssize_t output_data(void * buf, ssize_t count)
{
	_ELEM_T *buf_e = (_ELEM_T *) buf;
	ssize_t size = count/sizeof(_ELEM_T);
	for(ssize_t i=0;i<size;i++)
	{
		cout<<buf_e[i]<<endl;
	}
}
#endif 
