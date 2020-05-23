#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <mpi.h>

const int INFINITY = 10000;       //当两点之间没有关联的时候，默认两点之间的权值为10000

//声明读取矩阵的函数Read_matrix
void Read_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);

//声明打印矩阵的函数Print_matrix
void Print_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);

//声明Floyd算法函数Floyd
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);

//声明函数Owner
int Owner(int k, int p, int n);

//声明函数Copy_row
void Copy_row(int local_mat[], int n, int p, int row_k[], int k);

//主函数
int main(int argc, char* argv[]) {
	int n=1000;               //图的顶点数
	double start1,end1;
	int i,p,my_rank;
	int* local_mat=NULL;  //为了更好的分配邻接矩阵，这里采用的是一维数组
	int **juzhen;         //定义二维数组
	MPI_Comm comm;         
	//为二维数组juzhen分配动态内存空间
	juzhen=(int **)malloc(n*sizeof(int *));
	for (i=0;i<n;i++)
	{
		juzhen[i]=(int *)malloc(n*sizeof(int));
	}
	local_mat = (int *)malloc(n*n*sizeof(int));   //为local_mat分配动态内存空间
	MPI_Init(&argc, &argv);  //初始化并行环境
	comm = MPI_COMM_WORLD;  
	MPI_Comm_size(comm, &p); //获取MPI的线程数量
	MPI_Comm_rank(comm, &my_rank);    //获取进程ID
	start1=MPI_Wtime();      //MPI开始起点计时
	MPI_Bcast(&n, 1, MPI_INT, 0, comm);  //将顶点数量广播给所有进程
	local_mat = (int *)malloc(n*n/p*sizeof(int));  

	Read_matrix(local_mat, n, my_rank, p, comm);       //0号进程读入邻接矩阵，然后分给所有进程

	/*0号进程打印邻接矩阵
	if (my_rank == 0) printf("We got\n");
	Print_matrix(local_mat, n, my_rank, p, comm);
	if (my_rank == 0) printf("\n");
	*/
	Floyd(local_mat, n, my_rank, p, comm);
	 /*经过Floyd算法之后，0号进程打印邻接矩阵
	 if(my_rank == 0) 
		 {
		 printf("The solution is:\n");
		 Print_matrix(local_mat, n, my_rank, p, comm);
	 }*/
	free(juzhen);            //释放矩阵juzhen的内存空间
	free(local_mat);        //释放矩阵local_mat的内存空间
	end1=MPI_Wtime();       //MPI结束终点计时
	if (my_rank==0)
		printf("当线程数量为%d,MPI编程toltaltime=%f",p,end1-start1);
	MPI_Finalize();        //退出MPI程序
	return 0;
}  

//定义邻接矩阵的读入函数
void Read_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) 
{ 
	int i,j;
	int* temp_mat = NULL;
	//当线程为0时，从文本文件中读取数据
	if (my_rank == 0) {
		temp_mat = (int *)malloc(n*n*sizeof(int));
		FILE* fp;
		fp = fopen("test1.txt", "r");
		if (!fp)
		{
			perror("cannot open file");
			//exit(-1);
		}
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
			{
				fscanf(fp,"%d", &temp_mat[i*n+j]);
			}
		/*for(i=0;i<n*n;i++)
		printf("%d ",temp_mat[i]);*/
		fclose(fp);
		//将读取的数据temp_mat按照线程数量分给所有线程
		MPI_Scatter(temp_mat, n*n/p, MPI_INT, local_mat, n*n/p, MPI_INT, 0, comm);
		free(temp_mat);
	} else {
		MPI_Scatter(temp_mat, n*n/p, MPI_INT,local_mat, n*n/p, MPI_INT, 0, comm);
	}
}  

//定义打印邻接矩阵的函数
void Print_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) {
					  int i, j;
					  int* temp_mat = NULL;

					  if (my_rank == 0) {
						  temp_mat = (int *)malloc(n*n*sizeof(int));
						  //从所有进程中收集数据，发送到0号进程
						  MPI_Gather(local_mat, n*n/p, MPI_INT, temp_mat, n*n/p, MPI_INT, 0, comm);
						  //为了打印效果更加直观，将没有两点之间没有关联（权值为10000）的打印成“i” 
						  for (i = 0; i < n; i++) {
						  for (j = 0; j < n; j++)
						  if (temp_mat[i*n+j] == INFINITY)
						  printf("i ");
						  else
						  printf("%d ", temp_mat[i*n+j]);
						  printf("\n");
						  }
						  free(temp_mat);
					  } else {
						  MPI_Gather(local_mat, n*n/p, MPI_INT, temp_mat, n*n/p, MPI_INT, 0, comm);
					  }
}  

//定义Floyd函数
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) {
	int global_k, local_i, global_j, temp;  //定义临时变量
	int root;
	int* row_k = (int *)malloc(n*sizeof(int));

	for (global_k = 0; global_k < n; global_k++) 
	{
		root = Owner(global_k, p, n);    //调用Owner方法
		if (my_rank == root)
			Copy_row(local_mat, n, p, row_k, global_k);   //调用Copy_row方法
		MPI_Bcast(row_k, n, MPI_INT, root, comm);     //将row_k进行广播
		//Floyd算法的核心，即判断是否存在local_mat[i][temp]+local_mat[k][i]<local_mat[i][j]
		for (local_i = 0; local_i < n/p; local_i++)
			for (global_j = 0; global_j < n; global_j++) {
				temp = local_mat[local_i*n + global_k] + row_k[global_j];
				if (temp < local_mat[local_i*n+global_j])
					local_mat[local_i*n + global_j] = temp;
			}
	}
	free(row_k);
} 

//定义Owner函数
int Owner(int k, int p, int n) {
	return k/(n/p);
} 

//定义Copy_row函数
void Copy_row(int local_mat[], int n, int p, int row_k[], int k) {
	int j;
	int local_k = k % (n/p);
	for (j = 0; j < n; j++)
		row_k[j] = local_mat[local_k*n + j];
}  
