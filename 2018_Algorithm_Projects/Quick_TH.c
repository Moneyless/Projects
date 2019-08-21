#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define TIDS 16	//스레드 최대 갯수
#define THRESHOLD 16	//정렬할 데이터의 값이 이보다 작으면 다른 정렬(삽입 정렬)을 수행
#define number_of_sorting_data 50000000	//전역변수 배열의 크기(정렬할 데이터의 갯수)

int data[number_of_sorting_data] = { 0 , };	//데이터를 입력받아 정렬하고 출력할 배열

void quicksort(int low, int high);
void partition(int low, int high, int *pivotpoint);
void insertionsort(int low, int high);
void * thsort(void *param);

static int div_num;	//데이터 갯수가 이보다 내려가면 스레드를 더 이상 생성하지 않음 (데이터 갯수 / 생성할 스레드 갯수)

typedef struct VALUE{	//스레드 함수(void * 함수)에 넘겨줄 매개변수
	int low;
	int high;
}VALUE;

__inline void partition(int low, int high, int *pivotpoint){
	//data[low]에서 data[high]까지를 pivot을 기준으로 둘로 나눔
	
	register int i, j, pivot, temp = 0;
	pivot = data[low];	//첫 번째 변수를 pivot으로 세움 (한 5개 만들어서 할걸...)
	j = low;
	
	for(i = low + 1; i <= high; i++){
		if(pivot > data[i]){
			j++;
			temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
	}
	*pivotpoint = j;
	temp = data[low];
	data[low] = data[*pivotpoint];
	data[*pivotpoint] = temp;
	
}

__inline void insertionsort(int low, int high){ //삽입정렬
	register int temp, i , j;
    for(i = low + 1; i <= high; i++){
		temp = data[i];
		j = i - 1;
		while(j >= low && data[j] > temp){	//앞에서부터 정렬하며 이미 정렬된 배열과 비교해서 자신의 위치를 찾아 삽입
			data[j+1] = data[j];
			j=j-1;
		}
		data[j+1] = temp;
	}
}

void * thsort(void *param){	//스레드를 사용한 quicksort 함수
	
	VALUE parameter = *(VALUE *)param;	//VALUE 구조체로 가져온 high, low값을 사용
	int high = parameter.high, low = parameter.low;
	int pivotpoint;
	
	if(high - low < THRESHOLD){	//데이터 갯수가 THRESHOLD값보다 작을 경우 삽입정렬 실행
		insertionsort(low, high);
		return;
	}
	
	partition(low, high, &pivotpoint);	//low부터 high까지를 2개로 나눔
	
	if(high - low + 1 > div_num){	//스레드 최대 제한에 걸리지 않았으면 이하 내용을 실행
		pthread_t tid;	//스레드 id
		VALUE base;	//스레드에 매개변수로 넘겨줄 high, low값
		
		base.low = low;
		base.high = pivotpoint - 1;
		
		//앞의 정렬만 새 스레드를 만들어 돌리고 뒤는 현재 스레드가 계속 진행
		pthread_create(&tid, NULL, thsort, (void *)&base); // == quicksort(low, pivotpoint-1); ①
		quicksort(pivotpoint+1, high);
		
		pthread_join(tid, NULL);	// ①에서 생성한 스레드가 끝나는걸 기다림
	}else{	//스레드 최대 제한에 걸렸을 경우 새 스레드를 생성하지 않고 현재 스레드에서 진행
		quicksort(low, pivotpoint-1);
		quicksort(pivotpoint+1, high);
	}
}

void quicksort(int low, int high){	//메인 스레드가 진행하는 quicksort 함수

	int pivotpoint;
	
	if(high - low < THRESHOLD){	//데이터 갯수가 THRESHOLD값보다 작을 경우 선택정렬 실행
		insertionsort(low, high);
		return;
	}
	
	partition(low, high, &pivotpoint);	//low부터 high까지를 2개로 나눔
	
	if(high - low + 1 > div_num){	//스레드 최대 제한에 걸리지 않았으면 이하 내용을 실행
		pthread_t tid;	//스레드 id
		VALUE base;	//스레드에 매개변수로 넘겨줄 high, low값
		
		base.low = low;
		base.high = pivotpoint - 1;
		
		//앞의 정렬만 새 스레드를 만들어 돌리고 뒤는 현재 스레드가 계속 진행
		pthread_create(&tid, NULL, thsort, (void *)&base);	// == quicksort(low, pivotpoint-1); ②
		quicksort(pivotpoint+1, high);
		
		pthread_join(tid, NULL);	//②에서 생성한 스레드가 끝나는걸 기다림
	}else{	//스레드 최대 제한에 걸렸을 경우 새 스레드를 생성하지 않고 현재 스레드에서 진행
		quicksort(low, pivotpoint-1);
		quicksort(pivotpoint+1, high);
	}
}

int main(int argc, char* argv[]){
	int count = 0;	//들어온 원소 갯수
	char temp[12] = {0, };	//%10d 형태로 저장된 숫자를 가져오기 위한 문자열
	char filename[100] = {'\0'};	//출력할 파일 이름
	
	struct timeval start, end, diff;	//시간 측정을 위한 변수
	
	FILE *input;	//입력 파일
	FILE *output;	//출력 파일
	
	//파일 입력받기
	printf("file opening\n");
	if((input = fopen(argv[1], "rt")) == NULL){
		printf("%s file open error\n", argv[1]);
		exit(-1);
	}
	while(fgets(temp, 12, input) != NULL){
		if(atoi(temp) != 0){
			data[count] = atoi(temp);
			count++;
		}
		if(count >= number_of_sorting_data) break;
	}
	fclose(input);
	printf("completed.\nnumber of data = %d\nsorting\n", count);
	
	div_num = count / TIDS;	//스레드 갯수 제한용 변수 (총 데이터 수 / 생성할 스레드 갯수)
	
	gettimeofday(&start, NULL);	//시간 측정 시작
	
	quicksort(0, count-1);
	
	gettimeofday(&end, NULL);	//시간 측정 종료
	
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	if(diff.tv_usec < 0){
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}

	printf("completed.\ntime elapsed : %ld.%6lds\nfile writing\n", diff.tv_sec, diff.tv_usec);
	
	sprintf(filename, "th%dQUICK_result_%s", count/10000, argv[1]);
	output = fopen(filename, "wa");
				
	int i = 0;
	for(i=0; i<count; i++){
		fprintf(output, "%10d ", data[i]);
		if((i+1) % 5 == 0) fprintf(output, "\n");
	}
	fclose(output);
	
	printf("completed.\nend program\n");

	return 0;
}
