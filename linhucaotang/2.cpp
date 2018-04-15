#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int m, p, n;
int threadnum;
double **matrix_c, **matrix_a, **matrix_b;

void init(double ** &matrix, int r, int c) {
    matrix = (double **) malloc(sizeof(double *) * r);
    matrix[0] = (double *) malloc(sizeof(double) * r * c);
    for (int i = 1; i < r; i++)
        matrix[i] = matrix[i - 1] + c;
}

void matrix_multiply() {
#pragma omp parallel 
	{
#pragma omp for schedule(dynamic)
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++) {
				matrix_c[i][j] = 0;
				for (int k = 0; k < p; k++)
					matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
			}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2)
		exit(1);
	threadnum = atoi(argv[1]);
	omp_set_num_threads(threadnum);
	
    int i, j;
    scanf("%d%d%d", &m, &p, &n);
    init(matrix_a, m, p);
    init(matrix_b, p, n);
    init(matrix_c, m, n);
    for (i = 0; i < m; i++)
        for (j = 0; j < p; j++)
            scanf("%lf", &matrix_a[i][j]);
    for (i = 0; i < p; i++)
        for (j = 0; j < n; j++)
            scanf("%lf", &matrix_b[i][j]);
    matrix_multiply();
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%f ", matrix_c[i][j]);
        printf("\n");
    }
    return 0;
}
