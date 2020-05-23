#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <mpi.h>

const int INFINITY = 10000;       //������֮��û�й�����ʱ��Ĭ������֮���ȨֵΪ10000

//������ȡ����ĺ���Read_matrix
void Read_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);

//������ӡ����ĺ���Print_matrix
void Print_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);

//����Floyd�㷨����Floyd
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);

//��������Owner
int Owner(int k, int p, int n);

//��������Copy_row
void Copy_row(int local_mat[], int n, int p, int row_k[], int k);

//������
int main(int argc, char* argv[]) {
	int n=1000;               //ͼ�Ķ�����
	double start1,end1;
	int i,p,my_rank;
	int* local_mat=NULL;  //Ϊ�˸��õķ����ڽӾ���������õ���һά����
	int **juzhen;         //�����ά����
	MPI_Comm comm;         
	//Ϊ��ά����juzhen���䶯̬�ڴ�ռ�
	juzhen=(int **)malloc(n*sizeof(int *));
	for (i=0;i<n;i++)
	{
		juzhen[i]=(int *)malloc(n*sizeof(int));
	}
	local_mat = (int *)malloc(n*n*sizeof(int));   //Ϊlocal_mat���䶯̬�ڴ�ռ�
	MPI_Init(&argc, &argv);  //��ʼ�����л���
	comm = MPI_COMM_WORLD;  
	MPI_Comm_size(comm, &p); //��ȡMPI���߳�����
	MPI_Comm_rank(comm, &my_rank);    //��ȡ����ID
	start1=MPI_Wtime();      //MPI��ʼ����ʱ
	MPI_Bcast(&n, 1, MPI_INT, 0, comm);  //�����������㲥�����н���
	local_mat = (int *)malloc(n*n/p*sizeof(int));  

	Read_matrix(local_mat, n, my_rank, p, comm);       //0�Ž��̶����ڽӾ���Ȼ��ָ����н���

	/*0�Ž��̴�ӡ�ڽӾ���
	if (my_rank == 0) printf("We got\n");
	Print_matrix(local_mat, n, my_rank, p, comm);
	if (my_rank == 0) printf("\n");
	*/
	Floyd(local_mat, n, my_rank, p, comm);
	 /*����Floyd�㷨֮��0�Ž��̴�ӡ�ڽӾ���
	 if(my_rank == 0) 
		 {
		 printf("The solution is:\n");
		 Print_matrix(local_mat, n, my_rank, p, comm);
	 }*/
	free(juzhen);            //�ͷž���juzhen���ڴ�ռ�
	free(local_mat);        //�ͷž���local_mat���ڴ�ռ�
	end1=MPI_Wtime();       //MPI�����յ��ʱ
	if (my_rank==0)
		printf("���߳�����Ϊ%d,MPI���toltaltime=%f",p,end1-start1);
	MPI_Finalize();        //�˳�MPI����
	return 0;
}  

//�����ڽӾ���Ķ��뺯��
void Read_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) 
{ 
	int i,j;
	int* temp_mat = NULL;
	//���߳�Ϊ0ʱ�����ı��ļ��ж�ȡ����
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
		//����ȡ������temp_mat�����߳������ָ������߳�
		MPI_Scatter(temp_mat, n*n/p, MPI_INT, local_mat, n*n/p, MPI_INT, 0, comm);
		free(temp_mat);
	} else {
		MPI_Scatter(temp_mat, n*n/p, MPI_INT,local_mat, n*n/p, MPI_INT, 0, comm);
	}
}  

//�����ӡ�ڽӾ���ĺ���
void Print_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) {
					  int i, j;
					  int* temp_mat = NULL;

					  if (my_rank == 0) {
						  temp_mat = (int *)malloc(n*n*sizeof(int));
						  //�����н������ռ����ݣ����͵�0�Ž���
						  MPI_Gather(local_mat, n*n/p, MPI_INT, temp_mat, n*n/p, MPI_INT, 0, comm);
						  //Ϊ�˴�ӡЧ������ֱ�ۣ���û������֮��û�й�����ȨֵΪ10000���Ĵ�ӡ�ɡ�i�� 
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

//����Floyd����
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) {
	int global_k, local_i, global_j, temp;  //������ʱ����
	int root;
	int* row_k = (int *)malloc(n*sizeof(int));

	for (global_k = 0; global_k < n; global_k++) 
	{
		root = Owner(global_k, p, n);    //����Owner����
		if (my_rank == root)
			Copy_row(local_mat, n, p, row_k, global_k);   //����Copy_row����
		MPI_Bcast(row_k, n, MPI_INT, root, comm);     //��row_k���й㲥
		//Floyd�㷨�ĺ��ģ����ж��Ƿ����local_mat[i][temp]+local_mat[k][i]<local_mat[i][j]
		for (local_i = 0; local_i < n/p; local_i++)
			for (global_j = 0; global_j < n; global_j++) {
				temp = local_mat[local_i*n + global_k] + row_k[global_j];
				if (temp < local_mat[local_i*n+global_j])
					local_mat[local_i*n + global_j] = temp;
			}
	}
	free(row_k);
} 

//����Owner����
int Owner(int k, int p, int n) {
	return k/(n/p);
} 

//����Copy_row����
void Copy_row(int local_mat[], int n, int p, int row_k[], int k) {
	int j;
	int local_k = k % (n/p);
	for (j = 0; j < n; j++)
		row_k[j] = local_mat[local_k*n + j];
}  
