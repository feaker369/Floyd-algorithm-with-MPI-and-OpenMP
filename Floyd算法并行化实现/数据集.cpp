#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
const int max =1000; //����Ҫ�����ڽӾ���Ķ����� 
int main()
{
	FILE* fp;
	int i ,j,i1,j1;
	int **a;
	//Ϊ��ά������䶯̬�ڴ�ռ� 
	a = (int **)malloc(max * sizeof(int *));
	for(i=0;i<max;i++)
	{
		a[i]=(int *)malloc(max * sizeof(int));
	}
	srand(time(NULL));     //�������ʱ������ 
	//��������ڽӾ����Ԫ��ֵ����Χ��1~100�� 
	for(i = 0; i < max ; i++)
	{
		for(j = 0 ;j < max ; j++)
		{
			a[i][j] = rand()%100+1;
			if(i == j)
				a[i][j]=0;
		}
	}
	
	//�����������c��Ȼ�����һ����СΪc�Ķ�̬�ڴ�ռ� 
	int c = rand()%100+50;
	int *col;
	col = (int *)malloc(c*sizeof(int));

    //�����������r��Ȼ�����һ����СΪr�Ķ�̬�ڴ�ռ� 
	int r = rand()%100+50;
	int *row;
	row = (int *)malloc(r*sizeof(int));
	
	//Ϊ����col[c]�������Ԫ��ֵ����Χ��0~max�� 
	for( i = 0 ;i < c;i++)
	{
		col[i]=rand()%max;
	}
	printf("\n");
	
	//Ϊ����row[r]�������Ԫ��ֵ����Χ��0~max�� 
	for( j = 0 ;j < r;j++)
	{
		row[j]=rand()%max;
	}
	//col[c]�е�Ԫ����Ϊ������row[r]�е�Ԫ����Ϊ����������Ӧ��Ȩֵ����Ϊ10000��������֮���޹����� 
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
				//�ڽӾ���ĶԽ���Ϊ0 
				if(i==j)
					a[i][j]=0;
		}
	//�����ɵ��ڽӾ��󱣴浽�ı��ļ���	
		fp = fopen("test1.txt", "w");
		if (!fp)
		{
			perror("cannot open file");
			//exit(-1);
		}

		/*�Ѷ�ά���������д���ļ�*/
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

