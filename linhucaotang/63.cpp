#include <cstdio>
#include <omp.h>
using namespace std;
int num[20000] = {1, 3, 2, 5, 4};
int tmp[20000] = {};
void merge(int beg, int end, int * ret) {
	if (beg == end)
		return ;
	int mid = (beg + end) >> 1, lpos = beg, rpos = mid + 1, 
		ml = 0 ,mr = 0 , i;

#pragma omp task shared(ml)
	merge(beg, mid, &ml); 
#pragma omp task shared(mr)
	merge(mid + 1, end, &mr);
#pragma omp taskwait
	*ret += (ml + mr);
#pragma omp parallel for
	for (i = beg; i <= end; ++i)
		tmp[i] = num[i];
	i = beg;
	while (lpos <= mid && rpos <= end)
		if (tmp[lpos] <= tmp[rpos])
			num[i++] = tmp[lpos++];
		else {
			num[i++] = tmp[rpos++];
			*ret += mid - lpos + 1;
		}
#pragma omp parallel sections
	{
#pragma omp section
		while (lpos <= mid)
			num[i++] = tmp[lpos++];
#pragma omp section
		while (rpos <= end)
			num[i++] = tmp[rpos++];
	}
}
int main() {
	int N = 0, ans = 0;
	scanf("%d", &N);
	for (int i = 0; i < N; ++i)
		scanf("%d", num + i);
#pragma omp parallel
	{
#pragma omp single
		merge(0,N-1,&ans);
	}
	printf("%d\n", ans);
	return 0;
}
