#include<stdio.h>
#include<stdlib.h>
#include <omp.h>
#include<string.h>
#include<time.h>

int main(int argc,char* argv[]){
	int max = 1000;      //max为邻接矩阵的顶点
	int i,j,v,k,w,thread_count;   
	thread_count=strtol(argv[1],NULL,10);	 //therad_count为线程数量
	int **P=NULL,**D=NULL,**array1;         //定义二维数组P,D以及array1
	
	//为二维数组array1分配动态内存空间
	array1  = (int **)malloc(max*sizeof(int*));
	for(i=0;i<max;i++)
	{
		array1[i]=(int *)malloc(max*sizeof(int));
	}

	//为二维数组P分配动态内存空间
	P  = (int **)malloc(max*sizeof(int*));
	for(i=0;i<max;i++)
	{
		P[i]=(int *)malloc(max*sizeof(int));
	}

	//为二维数组D分配动态内存空间
	D  = (int **)malloc(max*sizeof(int*));
	for(i=0;i<max;i++)
	{
		D[i]=(int *)malloc(max*sizeof(int));
	}

	double start2=omp_get_wtime();    //开始计时
	
	//从文本文件中读取数据
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

	fclose(fp);  //关闭文件流

//在共享内存编程中初始化二维矩阵P,D
#pragma omp parallel for num_threads(thread_count)default(none) shared(array1,P,D,max) private(v,k,w)
	
	for(v = 0; v < max; v++){
		for(w = 0; w < max; w++){
			D[v][w] = array1[v][w];
			P[v][w] = w;
		}
	}

//在共享内存中进行Floyd算法
#pragma omp parallel for num_threads(thread_count)default(none) shared(array1,P,D,max) private(v,k,w)
	for(k = 0; k < max; k++){
		//v为起点 
		for(v = 0 ; v < max; v++){
			//w为终点 
			for(w =0; w < max; w++){
				if(D[v][w] > (D[v][k] + D[k][w])){
					D[v][w] = D[v][k] + D[k][w];//更新最小路径 
					P[v][w] = P[v][k];//更新最小路径中间顶点 
				}
			}
		}
	}
	double end2=omp_get_wtime();  //结束计时
	printf("totaltime=%f\n",thread_count,end2-start2);
}
