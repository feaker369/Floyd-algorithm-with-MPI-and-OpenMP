#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define MAXN 1000 
#define INF = 10000

typedef struct struct_graph{
    char vexs[MAXN];
    int vexnum;//������ 
    int edgnum;//���� 
    int **matirx;//�ڽӾ��� 
} Graph;



void short_path_floyd(Graph G, int **P, int **D){
    int v, w, k;
    //��ʼ��floyd�㷨���������� 
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            D[v][w] = G.matirx[v][w];
            P[v][w] = w;
        }
    }

    //�����Ǹ��������㷨�ĺ��Ĳ��� 
    //kΪ�м�� 
    for(k = 0; k < G.vexnum; k++){
        //vΪ��� 
        for(v = 0 ; v < G.vexnum; v++){
            //wΪ�յ� 
            for(w =0; w < G.vexnum; w++){
                if(D[v][w] > (D[v][k] + D[k][w])){
                    D[v][w] = D[v][k] + D[k][w];//������С·�� 
                    P[v][w] = P[v][k];//������С·���м䶥�� 
                }
            }
        }
    }
/* 
    printf("\n��ʼ����D����\n");
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            printf("%d ", D[v][w]);
        }
        printf("\n");
    }

    printf("\n��ʼ����P����\n");
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            printf("%d", P[v][w]);
        }
        printf("\n");
    }

    v = 0;
    w = 9;
    //�� 0 �� 3����С·��
    printf("\n%d -> %d ����С·��Ϊ��%d\n", v, w, D[v][w]);
    k = P[v][w];
    printf("path: %d", v);//��ӡ���
    while(k != w){
        printf("-> %d", k);//��ӡ�м��
        k = P[k][w]; 
    }
    printf("-> %d\n", w);
    */ 
}

int main(){
	int **pathmatirx;//��¼��Ӧ�����С·����ǰ���㣬����p(1,3) = 2 ˵������1������3����С·��Ҫ����2 
int **shortPath;//��¼��������С·��ֵ
long start,end;
    int v, w,i,j;
    Graph G;
    G.vexnum=MAXN;
    //���䶯̬�ڴ�ռ� 
	pathmatirx = (int **)malloc(MAXN * sizeof(int *));
	for(i=0;i<MAXN;i++)
	{
		pathmatirx[i]=(int *)malloc(MAXN * sizeof(int));
	}

	shortPath = (int **)malloc(MAXN * sizeof(int *));
	for(i=0;i<MAXN;i++)
	{
		shortPath[i]=(int *)malloc(MAXN * sizeof(int));
	}
	G.matirx= (int **)malloc(MAXN * sizeof(int *));
	for(i=0;i<MAXN;i++)
	{
		G.matirx[i]=(int *)malloc(MAXN * sizeof(int));
	}
	start = clock();
    FILE* fp;
	fp = fopen("test1.txt", "r");
	if (!fp)
	{
		perror("cannot open file");
		//exit(-1);
	}
	for (i = 0; i < MAXN; i++)
	{
		for (j = 0; j < MAXN; j++)
		{
			fscanf(fp, "%d ", &G.matirx[i][j]);

		}
	}
	fclose(fp);
   /* 
    printf("\n����ľ���ֵ��\n");
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            printf("%d ", G.matirx[v][w]);
        }
        printf("\n");
    }
    */
	 
    short_path_floyd(G, pathmatirx, shortPath);
    end = clock();
    printf("��������totaltime:%ld:����",end-start); 
	return 0;
}
