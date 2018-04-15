#include <iostream>
#include <omp.h>
using namespace std;
int mid[20000], back[20000], res[20000];
void front(int start, int end, int bias, int pos) {
	if (start <= end) {
		res[pos] = back[end];
		int root,tmp;
#pragma omp parallel
		{
#pragma omp for schedule(static,2)
			for (tmp = start; tmp <= end; ++tmp)
				if (mid[tmp + bias] == back[end])
					root = tmp;
#pragma omp task
			front(start, root - 1, bias, pos + 1);
#pragma omp task
			front(root, end - 1, bias + 1, pos + 1 + root - start);
		}
	}
}
int main() {
	int len;
	for (len = 0; cin.peek() != '\n'; ++len)
		cin >> mid[len];
	for (int i = 0; i < len; ++i)
		cin >> back[i];
	front(0, len - 1, 0, 0);
	for (int i = 0; i < len; ++i) {
		cout << res[i] << ' ';
	}
	return 0;
}
