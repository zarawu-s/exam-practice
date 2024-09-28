/*
	MPI program that multiplies matrix A (m x n) and matrix B (n x k) resulting in the product matrix C (m x k).
	- the program also finds the product of each row of matrix C
	- the multiplication is done by having the master process send the entire matrix A to each process and k/p columns of matrix B to each process (p is the number of processes, k is divisible by p)
	- all processes participate in the computation
	- the final result of the multiplication is stored in the master process, which also displays it
	- ensure that the k/p columns of matrix B are sent to each process at once, directly from matrix B
	- the task should be implemented using collective operations, except in cases where Point-to-Point operations are necessary	
*/

#include <mpi.h>
#include <stdio.h>
#include <malloc.h>

#define M 2
#define N 3
#define K 8

#define local_size 2 //  K/size for 4 processes

int main(int argc, char** argv)
{
	int rank, size, master = 0;
	int A[M][N], B[N][K], C[M][K];

	int local_B[N][local_size], local_C[M][local_size];
	int row_prod[M], local_row_prod[M];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Request request;
	MPI_Status status;

	if (rank == master)
	{
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				A[i][j] = (i + j) / 2;

		for (int i = 0; i < N; i++)
			for (int j = 0; j < K; j++)
				B[i][j] = i + j % 2 ;

		printf("Matrix A (%d x %d):\n", M, N);
		for (int i = 0; i < M; i++)
		{
			printf("|\t");
			for (int j = 0; j < N; j++)
				printf("%d\t", A[i][j]);
			printf("|\n");
		}

		printf("Matrix B (%d x %d):\n", N, K);
		for (int i = 0; i < N; i++)
		{
			printf("|\t");
			for (int j = 0; j < K; j++)
				printf("%d\t", B[i][j]);
			printf("|\n");
		}
	}

	MPI_Bcast(&A[0][0], M * N, MPI_INT, master, MPI_COMM_WORLD);
	
	if (rank == master)
	{
		for (int i = 1; i < size; i++)
		{
			for (int j = 0; j < N; j++)
				for (int q = 0; q < local_size; q++)
					local_B[j][q] = B[j][i * local_size + q];

			MPI_Send(&local_B[0][0], N * local_size, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		for (int i = 0; i < N; i++)
			for (int j = 0; j < local_size; j++)
				local_B[i][j] = B[i][j];
	}
	else {
		MPI_Recv(&local_B[0][0], N * local_size, MPI_INT, master, 0, MPI_COMM_WORLD, &status);
	}
	

	for (int i = 0; i < M; i++)
	{
		local_row_prod[i] = 1;
		for (int q = 0; q < local_size; q++)
		{
			local_C[i][q] = 0;
			for (int j = 0; j < N; j++)
				local_C[i][q] += A[i][j] * local_B[j][q];
			local_row_prod[i] *= local_C[i][q];
		}
	}

	for (int i = 0; i < size; i++)
		for (int j = 0; j < M; j++)
			MPI_Isend(&local_C[j][0], local_size, MPI_INT, master, 0, MPI_COMM_WORLD, &request);

	MPI_Reduce(&local_row_prod[0], &row_prod[0], M, MPI_INT, MPI_PROD, master, MPI_COMM_WORLD);

	if (rank == master)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < M; j++)
				MPI_Recv(&C[j][i * local_size], local_size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

	if (rank == master)
	{
		printf("Matrix C (%d x %d):\n", M, K);
		for (int i = 0; i < M; i++)
		{
			printf("|\t");
			for (int j = 0; j < K; j++)
				printf("%d\t", C[i][j]);
			printf("|\n");
		}

		printf("Product of each row in matrix C is: \n");
		for (int i = 0; i < M; i++)
			printf("Row %d: %d\t", i, row_prod[i]);
		printf("\n");
	}

	MPI_Finalize();
	return 0;
}