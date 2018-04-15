#include <iostream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

#ifdef LOCAL
size_t	output_data(uint64_t *buf,size_t size)
{
	uint64_t s = size/sizeof(uint64_t);
/*	for(uint64_t i=0;i<s;i++)
	{
		std::cout<<buf[i]<<" ";
		if(i && i%20 == 0) std::cout<<std::endl;
	}*/
	return 0;
}
#else
#include "fio.h"
#endif

#define SYNCHRONIZED(lock)	pthread_mutex_lock(&lock);
#define SYN_END(lock)		pthread_mutex_unlock(&lock);
#define PRIME				false 
#define N_PRIME				true
using namespace std;

uint64_t scope;
bool *buffer;
int processes;
int currPrime;
vector<uint64_t> ans;

void *work(void * arg)
{
	uint64_t start = (uint64_t)arg;
	start *= currPrime;
	int stride = processes*currPrime;
	for(uint64_t i = start;i<scope;i+=stride)
		buffer[i] = N_PRIME;
}

int main(int argc, char * argv[])
{
	if(argc!=3) 
	{
		fprintf(stderr,"Usage : %s <processes> <k>\n",argv[0]);
		exit(0);
	}
	processes = atoi(argv[1]);
	int n = atoi(argv[2]);
	scope = 1ull<<n;
	buffer = new bool[scope];
	memset(buffer,PRIME,scope);

	buffer[2] = PRIME;


	pthread_t tids[processes];
	for(uint64_t i=2;i<scope;++i)
	{
		if(buffer[i] == N_PRIME) continue;
		currPrime = i;
		ans.push_back(i);
		/* create slaves */
		for(int p = 0;p<processes;p++)
			pthread_create(tids+p,NULL,work,(void*)((uint64_t)p));
		
		/* wait */
		for(int p = 0;p < processes; p++)
			pthread_join(tids[p], NULL);	
	}

#ifdef LOCAL 
	output_data(&ans[0],sizeof(uint64_t)*ans.size());
#endif
	return 0;
}
