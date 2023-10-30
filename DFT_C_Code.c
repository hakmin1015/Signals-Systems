#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592653589793238

typedef struct complex{
	double Re, Im;
}COMPLEX;

int main(void){
	FILE *fp;
	fp = fopen("sample1.snd", "rb");
	if(fp == NULL){
		fprintf(stderr, "sample1.snd 파일을 열 수 없습니다.\n");
		return 1;
	}				// sample1.snd 파일을 열 수 없을 때 에러 메시지 출력 후 종료. 
	
	int WINDOW_SIZE = 0;
	printf("Window size를 입력하시오 : ");
	scanf("%d", &WINDOW_SIZE);
	
	COMPLEX* complex;
	int k=0,n=0;
	char original_file[WINDOW_SIZE];						// signed 8bit 파일이므로 1byte크기를 가지는 char형으로 선언.
	double* mul_window = NULL;
	int choose_window = 0;
	double spectrum_size[WINDOW_SIZE];
	int max_k = 0;
	double max_freq = 0;
	
	mul_window = (double*)malloc(sizeof(double) * WINDOW_SIZE);
	complex = (COMPLEX*)malloc(sizeof(COMPLEX) * WINDOW_SIZE);		// 동적 할당. 
	
	printf("어떤 Window를 씌울지 고르시오(Hamming Window : 1 , Rectangular Window : 2) : ");
	scanf("%d", &choose_window);
	
	switch(choose_window)
	{
		case 1 : 				// Hamming Window 적용 
			printf("\nHamming Window를 사용합니다.\n");
			for(n=0; n<WINDOW_SIZE; n++){
				fread(original_file , sizeof(char), WINDOW_SIZE, fp);		// sample1.snd 파일의 값을 읽어와 original_file 변수에 넣음. 
				mul_window[n] = (double)original_file[n] * (0.54-0.46*cos((2*PI*n)/WINDOW_SIZE));		// 원래 값에 hamming window를 씌움. 
				complex[n].Re = 0;						// 실수부 초기화 
				complex[n].Im = 0;						// 허수부 초기화 
			}
			break;
		
		case 2 : 				// Rectangular Window 적용 
			printf("\nRectangular Window를 사용합니다.\n");
			for(n=0; n<WINDOW_SIZE; n++){
				fread(original_file , sizeof(char), WINDOW_SIZE, fp);		// sample1.snd 파일의 값을 읽어와 original_file 변수에 넣음. 
				mul_window[n] = (double)original_file[n] * 1;		// 원래 값에 rectangular window를 씌움. 
				complex[n].Re = 0;						// 실수부 초기화 
				complex[n].Im = 0;						// 허수부 초기화 
			}
			break;
			
		default :
			printf("\n잘못 입력하였습니다. 프로그램을 종료합니다.\n");
			return 2;
	}

	
	for(k=0; k<WINDOW_SIZE; k++)
		for(n=0; n<WINDOW_SIZE; n++){
			complex[k].Re += mul_window[n] * (cos((2*PI*k*n)/WINDOW_SIZE));					// cos 계수 추출 -> 실수부
			complex[k].Im += mul_window[n] * (-(sin((2*PI*k*n)/WINDOW_SIZE)));				// sin 계수 추출 -> 허수부 (오일러 공식 e^(j*theta) = cos(theta) + j*sin(theta) 을 이용함)
		}
	
	for(k=0; k<WINDOW_SIZE; k++)
		spectrum_size[k] = sqrt(pow(complex[k].Re, 2) + pow(complex[k].Im, 2));				// 스펙트럼 크기를 구함. (a+jb꼴에서 sqrt(a^2+b^2)이 크기임.) 
		
	for(k=0; k<(WINDOW_SIZE)/2; k++)				// 대칭이므로 전체 WINDOW_SIZE에서 절반만 확인함. 
		if(spectrum_size[k]>max_freq){
			max_freq = spectrum_size[k];
			max_k = k;
		}
	
	max_freq = 4000*(max_k/(0.5*WINDOW_SIZE));					// 최대 크기를 가지는 주파수 값을 구하는 식 
	
	printf("\n%d points DFT 수행\n", WINDOW_SIZE);
	printf("\n최대 크기를 가지는 주파수 값 : %0.2lf\n\n\n", max_freq);
	printf("\n      k                              X[k]                                |X[k]|\n");
	printf("\n   ================================================================================\n\n");
	
	for(k=0; k<WINDOW_SIZE; k++)
		printf("   %4d                     (%lf)+j(%lf)                     %lf\n", k, complex[k].Re, complex[k].Im, spectrum_size[k]);
		
	fclose(fp);
	free(mul_window);
	free(complex);
	
	return 0;
}
