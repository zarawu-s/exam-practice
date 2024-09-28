/*
	MPI program that sends n elements of array X to all processes. 
	- X[n], n = p, where p is number of processes
	- array X is initialized in process with rank 2
	- using group operations, create array Y, yi = (p*(p+1)/2) * xi, i = 0, n-1 and p - number of processes	
*/

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#define N 8

int main(int argc, char** argv)
{
	int rank, size;
	int X[N], Y[N], temp[N];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 2)
	{
		for (int i = 0; i < N; i++)
			X[i] = i + 1;
	}

	MPI_Bcast(&X[0], N, MPI_INT, 2, MPI_COMM_WORLD);

	//this way we get p * xi
	MPI_Reduce(&X[0], &temp[0], N, MPI_INT, MPI_SUM, 2, MPI_COMM_WORLD);

	//instead of:
	/*if (rank == 2)
	{
		for (int i = 0; i < N; i++)
			Y[i] = temp[i] * (size + 1) / 2;
	}*/
	//we can use Bcast + Reduce

	MPI_Bcast(&temp[0], N, MPI_INT, 2, MPI_COMM_WORLD);

	//this way we get p * (p * xi)
	MPI_Reduce(&temp[0], &Y[0], N, MPI_INT, MPI_SUM, 2, MPI_COMM_WORLD);
	//(p * (p * xi) + (p * xi)) / 2
	if (rank == 2)
	{
		for (int i = 0; i < N; i++)
		{
			Y[i] += temp[i];
			Y[i] /= 2;
		}
	}

	if (rank == 2)
	{
		printf("Array X = {\t");
		for (int i = 0; i < N; i++)
			printf("%d \t", X[i]);
		printf("}\n");

		printf("Array Y = {\t");
		for (int i = 0; i < N; i++)
			printf("%d \t", Y[i]);
		printf("}\n");
	}
		
	MPI_Finalize();
	return 0;
}