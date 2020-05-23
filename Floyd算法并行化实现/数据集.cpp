#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
const int max =1000; //定义要生成邻接矩阵的顶点数 
int main()
{
	FILE* fp;
	int i ,j,i1,j1;
	int **a;
	//为二维数组分配动态内存空间 
	a = (int **)malloc(max * sizeof(int *));
	for(i=0;i<max;i++)
	{
		a[i]=(int *)malloc(max * sizeof(int));
	}
	srand(time(NULL));     //分配随机时间种子 
	//随机生成邻接矩阵的元素值（范围在1~100） 
	for(i = 0; i < max ; i++)
	{
		for(j = 0 ;j < max ; j++)
		{
			a[i][j] = rand()%100+1;
			if(i == j)
				a[i][j]=0;
		}
	}
	
	//随机生成整形c，然后分配一个大小为c的动态内存空间 
	int c = rand()%100+50;
	int *col;
	col = (int *)malloc(c*sizeof(int));

    //随机生成整形r，然后分配一个大小为r的动态内存空间 
	int r = rand()%100+50;
	int *row;
	row = (int *)malloc(r*sizeof(int));
	
	//为数组col[c]随机生成元素值（范围在0~max） 
	for( i = 0 ;i < c;i++)
	{
		col[i]=rand()%max;
	}
	printf("\n");
	
	//为数组row[r]随机生成元素值（范围在0~max） 
	for( j = 0 ;j < r;j++)
	{
		row[j]=rand()%max;
	}
	//col[c]中的元素作为行数，row[r]中的元素作为列数，将对应的权值设置为10000（即两点之间无关联） 
	printf("\n");
	for(i = 0;i < max ;i++)
		for(j = 0;j < max;j++)
		{
			for(i1 = 0;i1 < c;i1++)
				if(i==col[i1])
				{
					for(j1 = 0;j1 < r;j1++)
					{
						if (j==row[j1])
						{
							a[i][j]=10000;
						}
					}
				}
				//邻接矩阵的对角线为0 
				if(i==j)
					a[i][j]=0;
		}
	//将生成的邻接矩阵保存到文本文件中	
		fp = fopen("test1.txt", "w");
		if (!fp)
		{
			perror("cannot open file");
			//exit(-1);
		}

		/*把二维数组的内容写入文件*/
		for (i = 0; i < max; i++)
		{
			for (j = 0; j < max; j++)
			{
				fprintf(fp, "%d ", a[i][j]);
			}
			fputc('\n', fp);
		}
		fclose(fp);

}

