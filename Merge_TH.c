#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define TIDS 16	//스레드 최대 갯수
#define THRESHOLD 16	//정렬할 데이터의 값이 이보다 작으면 다른 정렬(선택 정렬)을 수행
#define number_of_sorting_data 50000000	//전역변수 배열의 크기(정렬할 데이터의 갯수)
	
int data[number_of_sorting_data] = {0, };	//데이터를 입력받아 정렬하고 출력할 배열
int result[number_of_sorting_data] = {0, };	//정렬된 데이터를 임시로 저장할 배열

void merge(int low, int mid, int high);
void mergesort(int low, int high);
void insertionsort(int low, int high);
void * thsort(void *param);

static int div_num;	//데이터 갯수가 이보다 내려가면 스레드를 더 이상 생성하지 않음 (데이터 갯수 / 생성할 스레드 갯수)

typedef struct VALUE{	//스레드 함수(void * 함수)에 넘겨줄 매개변수
	int low;
	int high;
}VALUE;

__inline void merge(int low, int mid, int high){	//low에서 high까지 합병
	register int i, j, k;
	i = low; j = mid + 1; k = low;
	while(i <= mid && j <= high){
		if(data[i] < data[j]){
			result[k] = data[i];
			i++;
		} else {
			result[k] = data[j];
			j++;
		}
		k++;
	}
	
	if(i > mid){	//앞쪽 배열이 뒤쪽 배열보다 빨리 끝났을 경우 뒤쪽 배열의 남은 원소를 남은 결과 배열에 저장
		for(; j <= high; j++, k++){
			result[k] = data[j];
		}
	}else{			//뒤쪽 배열이 앞쪽 배열보다 ~
		for(; i <= mid; i++, k++){
			result[k] = data[i];
		}
	}
	
	for(i = low; i <= high; i++){	//result에 저장된 정렬된 배열을 data에 복사
		data[i] = result[i];
	}
}

__inline void insertionsort(int low, int high){ 	//삽입정렬
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

void * thsort(void *param){	//스레드를 사용한 mergesort 함수
	
	VALUE parameter = *(VALUE *)param;	//VALUE 구조체로 가져온 high, low값을 사용
	int high = parameter.high, low = parameter.low;
	
	if(low < high){
		if(high - low + 1 < THRESHOLD){	//데이터 갯수가 THRESHOLD값보다 작을 경우 선택정렬 실행
			insertionsort(low, high);
		}
		int mid = (high + low) / 2;
		
		if(high - low + 1 > div_num){	//스레드 최대 제한에 걸리지 않았으면 이하 내용을 실행
			pthread_t tid;	//스레드 id
			VALUE base;	//스레드에 매개변수로 넘겨줄 high, low값
			
			base.low = low;
			base.high = mid;
			
			//앞의 정렬만 새 스레드를 만들어 돌리고 뒤는 현재 스레드가 계속 진행
			pthread_create(&tid, NULL, thsort, (void *) &base);	// == mergesort(low, mid); ①
			mergesort(mid + 1, high);
			
			pthread_join(tid, NULL);	// ①에서 생성한 스레드가 끝나는걸 기다림
		}else{	//스레드 최대 제한에 걸렸을 경우 새 스레드를 생성하지 않고 현재 스레드에서 진행
			mergesort(low, mid);	
			mergesort(mid + 1, high);
		}
		merge(low, mid, high);	//합병
	}
}

void mergesort(int low, int high){	//메인 스레드가 진행하는 mergesort 함수

	if(low < high){
		if(high - low + 1 < THRESHOLD){	//데이터 갯수가 THRESHOLD값보다 작을 경우 선택정렬 실행
			insertionsort(low, high);
		}
		int mid = (high + low) / 2;
		
		if(high - low + 1 > div_num){	//스레드 최대 제한에 걸리지 않았으면 이하 내용을 실행
			pthread_t tid;	//스레드 id
			VALUE base;	//스레드에 넘겨줄 high, low값
			
			base.low = low;
			base.high = mid;
			
			//앞의 정렬만 새 스레드를 만들어 돌리고 뒤는 현재 스레드가 계속 진행
			pthread_create(&tid, NULL, thsort, (void *) &base);	// == mergesort(low, mid); ②
			mergesort(mid + 1, high);
			
			pthread_join(tid, NULL);	// ②에서 생성한 스레드가 끝나는걸 기다림
		}else{	//스레드 최대 제한에 걸렸을 경우 새 스레드를 생성하지 않고 현재 스레드에서 진행
			mergesort(low, mid);
			mergesort(mid + 1, high);
		}
		merge(low, mid, high);	//합병
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
	while(fgets(temp, 12 * sizeof(char), input) != NULL){
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
	
	mergesort(0, count-1);
	
	gettimeofday(&end, NULL);	//시간 측정 종료
	
	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;

	if(diff.tv_usec < 0){
		diff.tv_sec = diff.tv_sec - 1;
		diff.tv_usec = diff.tv_usec + 1000000;
	}
	printf("completed.\ntime elapsed : %ld.%6lds\nfile writing\n", diff.tv_sec, diff.tv_usec);
	
	//파일 출력하기
	sprintf(filename, "th%dMERGE_result_%s", count/10000 ,argv[1]);
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