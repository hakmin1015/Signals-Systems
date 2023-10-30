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
		fprintf(stderr, "sample1.snd ������ �� �� �����ϴ�.\n");
		return 1;
	}				// sample1.snd ������ �� �� ���� �� ���� �޽��� ��� �� ����. 
	
	int WINDOW_SIZE = 0;
	printf("Window size�� �Է��Ͻÿ� : ");
	scanf("%d", &WINDOW_SIZE);
	
	COMPLEX* complex;
	int k=0,n=0;
	char original_file[WINDOW_SIZE];						// signed 8bit �����̹Ƿ� 1byteũ�⸦ ������ char������ ����.
	double* mul_window = NULL;
	int choose_window = 0;
	double spectrum_size[WINDOW_SIZE];
	int max_k = 0;
	double max_freq = 0;
	
	mul_window = (double*)malloc(sizeof(double) * WINDOW_SIZE);
	complex = (COMPLEX*)malloc(sizeof(COMPLEX) * WINDOW_SIZE);		// ���� �Ҵ�. 
	
	printf("� Window�� ������ ���ÿ�(Hamming Window : 1 , Rectangular Window : 2) : ");
	scanf("%d", &choose_window);
	
	switch(choose_window)
	{
		case 1 : 				// Hamming Window ���� 
			printf("\nHamming Window�� ����մϴ�.\n");
			for(n=0; n<WINDOW_SIZE; n++){
				fread(original_file , sizeof(char), WINDOW_SIZE, fp);		// sample1.snd ������ ���� �о�� original_file ������ ����. 
				mul_window[n] = (double)original_file[n] * (0.54-0.46*cos((2*PI*n)/WINDOW_SIZE));		// ���� ���� hamming window�� ����. 
				complex[n].Re = 0;						// �Ǽ��� �ʱ�ȭ 
				complex[n].Im = 0;						// ����� �ʱ�ȭ 
			}
			break;
		
		case 2 : 				// Rectangular Window ���� 
			printf("\nRectangular Window�� ����մϴ�.\n");
			for(n=0; n<WINDOW_SIZE; n++){
				fread(original_file , sizeof(char), WINDOW_SIZE, fp);		// sample1.snd ������ ���� �о�� original_file ������ ����. 
				mul_window[n] = (double)original_file[n] * 1;		// ���� ���� rectangular window�� ����. 
				complex[n].Re = 0;						// �Ǽ��� �ʱ�ȭ 
				complex[n].Im = 0;						// ����� �ʱ�ȭ 
			}
			break;
			
		default :
			printf("\n�߸� �Է��Ͽ����ϴ�. ���α׷��� �����մϴ�.\n");
			return 2;
	}

	
	for(k=0; k<WINDOW_SIZE; k++)
		for(n=0; n<WINDOW_SIZE; n++){
			complex[k].Re += mul_window[n] * (cos((2*PI*k*n)/WINDOW_SIZE));					// cos ��� ���� -> �Ǽ���
			complex[k].Im += mul_window[n] * (-(sin((2*PI*k*n)/WINDOW_SIZE)));				// sin ��� ���� -> ����� (���Ϸ� ���� e^(j*theta) = cos(theta) + j*sin(theta) �� �̿���)
		}
	
	for(k=0; k<WINDOW_SIZE; k++)
		spectrum_size[k] = sqrt(pow(complex[k].Re, 2) + pow(complex[k].Im, 2));				// ����Ʈ�� ũ�⸦ ����. (a+jb�ÿ��� sqrt(a^2+b^2)�� ũ����.) 
		
	for(k=0; k<(WINDOW_SIZE)/2; k++)				// ��Ī�̹Ƿ� ��ü WINDOW_SIZE���� ���ݸ� Ȯ����. 
		if(spectrum_size[k]>max_freq){
			max_freq = spectrum_size[k];
			max_k = k;
		}
	
	max_freq = 4000*(max_k/(0.5*WINDOW_SIZE));					// �ִ� ũ�⸦ ������ ���ļ� ���� ���ϴ� �� 
	
	printf("\n%d points DFT ����\n", WINDOW_SIZE);
	printf("\n�ִ� ũ�⸦ ������ ���ļ� �� : %0.2lf\n\n\n", max_freq);
	printf("\n      k                              X[k]                                |X[k]|\n");
	printf("\n   ================================================================================\n\n");
	
	for(k=0; k<WINDOW_SIZE; k++)
		printf("   %4d                     (%lf)+j(%lf)                     %lf\n", k, complex[k].Re, complex[k].Im, spectrum_size[k]);
		
	fclose(fp);
	free(mul_window);
	free(complex);
	
	return 0;
}
