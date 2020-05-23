#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define MAXN 1000 
#define INF = 10000

typedef struct struct_graph{
    char vexs[MAXN];
    int vexnum;//顶点数 
    int edgnum;//边数 
    int **matirx;//邻接矩阵 
} Graph;



void short_path_floyd(Graph G, int **P, int **D){
    int v, w, k;
    //初始化floyd算法的两个矩阵 
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            D[v][w] = G.matirx[v][w];
            P[v][w] = w;
        }
    }

    //这里是弗洛伊德算法的核心部分 
    //k为中间点 
    for(k = 0; k < G.vexnum; k++){
        //v为起点 
        for(v = 0 ; v < G.vexnum; v++){
            //w为终点 
            for(w =0; w < G.vexnum; w++){
                if(D[v][w] > (D[v][k] + D[k][w])){
                    D[v][w] = D[v][k] + D[k][w];//更新最小路径 
                    P[v][w] = P[v][k];//更新最小路径中间顶点 
                }
            }
        }
    }
/* 
    printf("\n初始化的D矩阵\n");
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            printf("%d ", D[v][w]);
        }
        printf("\n");
    }

    printf("\n初始化的P矩阵\n");
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            printf("%d", P[v][w]);
        }
        printf("\n");
    }

    v = 0;
    w = 9;
    //求 0 到 3的最小路径
    printf("\n%d -> %d 的最小路径为：%d\n", v, w, D[v][w]);
    k = P[v][w];
    printf("path: %d", v);//打印起点
    while(k != w){
        printf("-> %d", k);//打印中间点
        k = P[k][w]; 
    }
    printf("-> %d\n", w);
    */ 
}

int main(){
	int **pathmatirx;//记录对应点的最小路径的前驱点，例如p(1,3) = 2 说明顶点1到顶点3的最小路径要经过2 
int **shortPath;//记录顶点间的最小路径值
long start,end;
    int v, w,i,j;
    Graph G;
    G.vexnum=MAXN;
    //分配动态内存空间 
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
    printf("\n输入的矩阵值：\n");
    for(v = 0; v < G.vexnum; v++){
        for(w = 0; w < G.vexnum; w++){
            printf("%d ", G.matirx[v][w]);
        }
        printf("\n");
    }
    */
	 
    short_path_floyd(G, pathmatirx, shortPath);
    end = clock();
    printf("串行所需totaltime:%ld:毫秒",end-start); 
	return 0;
}
