/*
	MPI program that sends n elements of array X to all processes. 
	- X[M], n != M, M = n*p, where p is number of processes
	- array X is initialized in process with rank 2
	- using one more group operation, create array Y, yi = (p*(p+1)/2) * xi, i = 0, n-1 and p - number of processes	
*/

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#define M 8

int main(int argc, char** argv)
{
	int rank, size;
	int X[M], Y[M];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int local_size = M / size;
	int* temp = (int*)malloc(sizeof(int) * local_size);

	if (rank == 2)
	{
		for (int i = 0; i < M; i++)
			X[i] = i + 1;
	}

	MPI_Scatter(&X[0], local_size, MPI_INT, &temp[0], local_size, MPI_INT, 2, MPI_COMM_WORLD);

	for (int i = 0; i < local_size; i++)
		temp[i] *= (size * (size + 1) / 2);

	MPI_Gather(&temp[0], local_size, MPI_INT, &Y[0], local_size, MPI_INT, 2, MPI_COMM_WORLD);

	if (rank == 2)
	{
		printf("Array X = {\t");
		for (int i = 0; i < M; i++)
			printf("%d \t", X[i]);
		printf("}\n");

		printf("Array Y = {\t");
		for (int i = 0; i < M; i++)
			printf("%d \t", Y[i]);
		printf("}\n");
	}
		
	free(temp);

	MPI_Finalize();
	return 0;
}