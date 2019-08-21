#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_WEIGHT 99	//한 엣지에서 나올 수 있는 가중치의 최대치
#define INF_RATE 5	//무한대가 나올 확률 1/n
#define INF 100000	//무한대

typedef struct EDGE{	//엣지 표시용 구조체
	int from;	//시작 노드
	int to;	//목표 노드
	int weight;	//엣지의 가중치
}EDGE;

		int dijkstra(int start, int end, int n, int** W){	//다익스트라 알고리즘
			int i, j, vnear, min, path, route, total = 0;		
			int *touch, *length;										
			int *G;	//경로 저장용 배열
			EDGE *F;	//엣지 저장용 구조체
			
			touch = (int *)calloc(sizeof(int), n);
			length = (int *)calloc(sizeof(int), n);
			G = (int *)calloc(sizeof(int), n - 1);
			F = (EDGE *)calloc(sizeof(EDGE), n - 1);
			
			for(i = 0; i < n; i++){	
				length[i] = W[start - 1][i];
				touch[i] = start - 1;
			}
			for(i = 0;  i < n - 1; i++){
				min = INF;
				for(j = 0;  j < n ; j++){
					if(0 < length[j] && length[j] < min){
						min = length[j];
						vnear = j;
					}
				}// j for 1
				F[i].from = touch[vnear];	//엣지가 출발하는 노드
				F[i].to = vnear;	//정해진 목표 노드
				F[i].weight = W[F[i].from][F[i].to];	//목표 노드로 향하는 엣지의 가중치
				for (j = 0; j < n; j++) {
					if (length[vnear] + W[vnear][j] < length[j]) {
						length[j] = length[vnear] + W[vnear][j];
						touch[j] = vnear;
					}
				}//j for 2
				length[vnear] = -1;
			}// i for
			G[0] = end - 1;	//목적지의 인덱스
			route = touch[end - 1];	
			for(i = 1; route != start - 1; i++){ //목적지부터 출발지 순서로 배열 G에 경로 입력
				G[i] = route;
				route = touch[route];
			}
			G[i] = route;
			total = W[G[i]][G[i-1]];
			for(i = i - 1;i >= 0; i--){	//역순으로 입력된 경로를 다시 역순으로 출력하여 바른 순서대로 경로 출력
				printf("-> v%d ", G[i] + 1);
				if(i > 0) total = total + W[G[i]][G[i-1]];	//경로의 총 가중치 계산
			}
			printf("\n");
			free(touch); free(length); free(F); free(G);
			return total;
		}

int main(){
	int i, j, Node, Weight, Path, Total;
	int **W = NULL;
	struct timeval start, end, diff;
	
	printf("노드 갯수를 입력하세요 : ");
	scanf("%d", &Node);
	
	W = (int **)calloc(sizeof(int *), Node);
	
	for(i = 0; i < Node; i++){	//랜덤으로 W 행렬 (가중치 행렬) 생성
		W[i] = (int *)calloc(sizeof(int *), Node);
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
	
	while(1){
		printf("찾는 경로의 시작 노드 번호와 끝 노드 번호를 차례대로 입력하세요\n");
		scanf("%d %d", &i, &j);
		if(i == 0 && j == 0) break;	//	0 0을 입력하면 프로그램 종료
		if(i > Node || i < 1 || j > Node || j < 1){
			printf("범위 내에서 입력하세요 (1 ~ %d)\n", Node);
			continue;
		}
		printf("v%d ", i);
		gettimeofday(&start, NULL);		//시간 측정
		Total = dijkstra(i , j, Node, W);	
		gettimeofday(&end, NULL);
		printf("경로 엣지의 가중치의 합 : %d\n", Total);
		
		diff.tv_sec = end.tv_sec - start.tv_sec;	//시간 측정용
		diff.tv_usec = end.tv_usec - start.tv_usec;

		if(diff.tv_usec < 0){
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
		}
	
		printf("걸린 시간 : %ld.%06lds\n", diff.tv_sec, diff.tv_usec);
	}

	//free(W);		// * free처리하면 오류남
	return 0;
}