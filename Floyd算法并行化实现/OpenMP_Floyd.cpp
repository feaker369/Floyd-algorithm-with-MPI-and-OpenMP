#include<stdio.h>
#include<stdlib.h>
#include <omp.h>
#include<string.h>
#include<time.h>

int main(int argc,char* argv[]){
	int max = 1000;      //maxΪ�ڽӾ���Ķ���
	int i,j,v,k,w,thread_count;   
	thread_count=strtol(argv[1],NULL,10);	 //therad_countΪ�߳�����
	int **P=NULL,**D=NULL,**array1;         //�����ά����P,D�Լ�array1
	
	//Ϊ��ά����array1���䶯̬�ڴ�ռ�
	array1  = (int **)malloc(max*sizeof(int*));
	for(i=0;i<max;i++)
	{
		array1[i]=(int *)malloc(max*sizeof(int));
	}

	//Ϊ��ά����P���䶯̬�ڴ�ռ�
	P  = (int **)malloc(max*sizeof(int*));
	for(i=0;i<max;i++)
	{
		P[i]=(int *)malloc(max*sizeof(int));
	}

	//Ϊ��ά����D���䶯̬�ڴ�ռ�
	D  = (int **)malloc(max*sizeof(int*));
	for(i=0;i<max;i++)
	{
		D[i]=(int *)malloc(max*sizeof(int));
	}

	double start2=omp_get_wtime();    //��ʼ��ʱ
	
	//���ı��ļ��ж�ȡ����
	FILE* fp;               
	fp = fopen("test1.txt", "r");
	if (!fp)
	{
		perror("cannot open file");
		//exit(-1);
	}
	for (i = 0; i < max; i++)
	{
		for (j = 0; j < max; j++)
		{
			fscanf(fp, "%d ", &array1[i][j]);
		}
	}

	fclose(fp);  //�ر��ļ���

//�ڹ����ڴ����г�ʼ����ά����P,D
#pragma omp parallel for num_threads(thread_count)default(none) shared(array1,P,D,max) private(v,k,w)
	
	for(v = 0; v < max; v++){
		for(w = 0; w < max; w++){
			D[v][w] = array1[v][w];
			P[v][w] = w;
		}
	}

//�ڹ����ڴ��н���Floyd�㷨
#pragma omp parallel for num_threads(thread_count)default(none) shared(array1,P,D,max) private(v,k,w)
	for(k = 0; k < max; k++){
		//vΪ��� 
		for(v = 0 ; v < max; v++){
			//wΪ�յ� 
			for(w =0; w < max; w++){
				if(D[v][w] > (D[v][k] + D[k][w])){
					D[v][w] = D[v][k] + D[k][w];//������С·�� 
					P[v][w] = P[v][k];//������С·���м䶥�� 
				}
			}
		}
	}
	double end2=omp_get_wtime();  //������ʱ
	printf("totaltime=%f\n",thread_count,end2-start2);
}
