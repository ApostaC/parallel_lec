#include <cstdio>
#include <algorithm>
#include <iostream>
using namespace std;
int a[5000000];

int t[2][5000000];
int task(int start, int end, int threshold, int p) {
	int lp = 0, rp = end - start;
#pragma omp parallel for
	for (int i = start; i <= end; ++i) {
		if (a[i] < threshold) {
			t[p][lp++] = a[i];
		} else {
			t[p][rp--] = a[i];
		}
	}
	return lp;
}

int k_find(int high, int k) {
	int middle = high / 2;
	int s1,s2;
#pragma omp parallel sections
	{
#pragma omp section 
		s1 = task(1, middle, a[0], 0);
#pragma omp section
		s2 = task(middle + 1, high, a[0], 1);
	}

	if (s1 + s2 >= k) {
#pragma omp parallel sections
		{
#pragma omp section 
#pragma omp parallel for
			for (int i = 0; i < s1; ++i) {
				a[i] = t[0][i];
			}
#pragma omp section
#pragma omp parallel for
			for (int i = 0; i < s2; ++i) {
				a[i + s1] = t[1][i];
			}
		}
		return k_find(s1 + s2 - 1, k);
	}
	if (s1 + s2 < k - 1) {
#pragma omp parallel sections
		{
#pragma omp section
#pragma omp parallel for
			for (int i = s1; i <= middle - 1; ++i) {
				a[i - s1] = t[0][i];
			}
#pragma omp section
#pragma omp parallel for
			for (int i = s2; i <= high - middle - 1; ++i) {
				a[i - s2 + middle - s1] = t[1][i];
			}
		}
		return k_find(high - s1 - s2 - 1, k - s1 - s2 - 1);
	}
	return a[0];
}

int main() {
	int n, k,ans;
	scanf("%d%d", &n, &k);
	for (int i = 0; i < n; i++) {
		scanf("%d", &a[i]);
	}

#pragma omp single
		ans = k_find(n-1,k);
	printf("%d\n", ans);
	return 0;
}
