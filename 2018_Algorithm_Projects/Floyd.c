#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_WEIGHT 99	//한 엣지에서 나올 수 있는 가중치의 최대치
#define INF_RATE 5	//무한대가 나올 확률 1/n
#define INF 100000 //무한대

int** floyd(int n, int** W, int** D, int** P){	//플로이드 알고리즘으로 경로를 알려주는 행렬 P,
	int i, j, k;													//정점들 사이의 최단거리를 알려주는 행렬 D를 출력.
	
	D = W;
	for(k = 0; k < n; k++){
		for(i = 0; i < n; i++){
			for(j = 0; j < n; j++){
				if((D[i][k] + D[k][j]) < D[i][j]){
					P[i][j] = k + 1;
					D[i][j] = D[i][k] + D[k][j];
				}
			}
		}
	}
	return D;
}

void path(int q, int r, int **P) {			//지나가는 경로를 출력하는 함수
	if (P[q - 1][r - 1] != 0) {					//두 노드가 연결되어있다면 바로 빠져나감
		path(q, P[q - 1][r - 1], P);		
		printf("v%d -> ", P[q - 1][r - 1]);		//빠져나온 이후에 올라오면서 경로 출력
		path(P[q - 1][r - 1], r, P);
	}
}

void print_array(int Node, int **A){		//2차원 배열을 출력하는 함수
	int i, j;
	
	for(i = 0; i < Node; i++, printf("\n")){
		for(j = 0; j < Node; j++){
			printf("%3d", A[i][j]);
		}
	}
}
	
		
int main(){
	int i, j, Node, Weight, Path;
	int **W = NULL, **D = NULL, **P = NULL;
	struct timeval start, end, diff;
	
	printf("노드 갯수를 입력하세요 : ");
	scanf("%d", &Node);
	
	W = (int **)calloc(sizeof(int *), Node);
	D = (int **)calloc(sizeof(int *), Node);
	P = (int **)calloc(sizeof(int *), Node);
	
	for(i = 0; i < Node; i++){		//랜덤으로 W 행렬 (가중치 행렬) 생성
		W[i] = (int *)calloc(sizeof(int *), Node);
		D[i] = (int *)calloc(sizeof(int *), Node);
		P[i] = (int *)calloc(sizeof(int *), Node);
		for(j = 0; j < Node; j++){
			if(i == j){
				W[i][j] = 0;
			}
			else{
				Path = rand() % INF_RATE;	//무한대로 만들 곳 지정
				if(Path == 0) Weight = 0; else Weight = rand() % MAX_WEIGHT + 1;
				if(Weight == 0) W[i][j] = INF; else W[i][j] = Weight;
			}
		}
	}
	
	/*printf("각 노드간의 가중치 W :\n");		//W 행렬 출력
	for(i = 0; i < Node; i++, printf("\n")){
		for(j = 0; j < Node; j++){
			if(W[i][j] == INF) printf("  ∞");
			else printf("%3d", W[i][j]);
		}
	}*/
	
	gettimeofday(&start, NULL);		//시간 측정용
	D = floyd(Node, W, D, P);
	gettimeofday(&end, NULL);
	
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	if(diff.tv_usec < 0){
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}
	
	/*printf("최단거리 행렬 D : \n");	//D 행렬 출력
	print_array(Node, D);
	
	printf("정보저장 행렬 P : \n");	//P 행렬 출력
	print_array(Node, P);*/
	
	printf("걸린 시간 : %ld.%06lds\n", diff.tv_sec, diff.tv_usec);
	
	while(1){
		printf("찾는 경로의 시작 노드 번호와 끝 노드 번호를 차례대로 입력하세요\n");
		scanf("%d %d", &i, &j);
		if(i == 0 && j == 0) break;	//0 0을 입력하면 프로그램 종료
		if(i > Node || j > Node){	//예외 처리용
			printf("범위 내에서 입력하세요 (1 ~ %d)\n", Node);
			continue;
		}
		printf("v%d -> ", i);	//시작 노드 출력
		path(i, j, P);		//경로 출력
		printf("v%d\n", j);	//끝 노드 출력
		printf("경로 엣지의 가중치의 합 : %d\n", D[i - 1][j - 1]); //이미 floyd에서 최소 거리를 다 구했으므로 그냥 D행렬의 값을 표시하기로 함
	}

	//free(W); free(D); free(P);	// * free처리하면 오류남
	return 0;
}