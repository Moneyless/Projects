#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAXSIZE 10000
#define INT_MAX 2147483637

int minmult(int n, int *d, int **P) //dynamic
{
	int i, j, k, temp1, temp2, diagonal;
	int **M;

	M = (int **)calloc(sizeof(int*), n); //2차원 배열 M 생성
	for (i = 0; i < n; i++)
		M[i] = (int *)calloc(sizeof(int), n);

	for (diagonal = 0; diagonal < n ; diagonal++) {
		for (i = 0; i < n - diagonal; i++) {
			j = i + diagonal;
			temp2 = INT_MAX;

			for (k = i; k < j; k++) {
				temp1 = (M[i][k] + M[k + 1][j] + d[i] * d[k+1] * d[j+1]);
				if (temp2 > temp1) {
					temp2 = temp1;
					M[i][j] = temp2;
					P[i][j] = k + 1;
				}
			}
	//		printf("%d ", temp2);
		}
	}
	return M[0][n-1];
}

void printarray(int n, int **P)
{ 
	int i, j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			printf("%3d", P[i][j]);
		printf("\n");
	}
}

void order(int i, int j, int **P) {
	int k;

	if (i == j) printf("A%d", i + 1);
	else {
		k = P[i][j] - 1;
		putchar('(');
		order(i, k, P);
		order(k + 1, j, P);
		putchar(')');
	}
}

int main()
{
	int **P;
	int d[MAXSIZE];
	int i, n;
	int value1, value2;
	struct timeval start, end, diff;

	for (i = 0; i < MAXSIZE; i++) {
		d[i] = (3 * i) + 1;
		if (3 * i > 15)
			d[i] = ((3 * i) % 20) + 1;
	}

	while(1){
		printf("please input the number of matrix : ");
		scanf("%d", &n);
		
		if(n ==0) break;

		P = (int **)calloc(sizeof(int*), n); //2차원 배열 P 생성
		for (i = 0; i < n; i++)
			P[i] = (int *)calloc(sizeof(int), n);

		gettimeofday(&start, NULL);
		value1 = minmult(n, d, P);
		gettimeofday(&end, NULL);
		
		diff.tv_sec = end.tv_sec - start.tv_sec;
		diff.tv_usec = end.tv_usec - start.tv_usec;

		if(diff.tv_usec < 0){
			diff.tv_sec = diff.tv_sec - 1;
			diff.tv_usec = diff.tv_usec + 1000000;
		}

		printf("행렬 %d개를 곱할 때 필요한 최소 연산 횟수는 %d입니다. \n",  n, value1);
		printf("동적 프로그래밍은 %ld.%06lds초 입니다. \n", diff.tv_sec, diff.tv_usec);
		printarray(n, P);
		/*puts("곱셈 순서는 다음과 같습니다.");
		order(0, n - 1, P);*/
		printf("\n");
	}
	free(P);
}