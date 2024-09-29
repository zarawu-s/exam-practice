/*
	MPI program that implements the MPI Scatter collective operation using P2P communication involving p processes arranged in a hypercube as shown in Figure 1.
	- the program proceeds in log2p steps
	- after the execution of the program, each process P (k = 0, p-1) should display its data
	- data exchange between processes is shown step by step in Figure 1
	- in parentheses, the values present in each process at the corresponding step are indicated

	Then, implement the MPI_Gather operation, also using P2P communication and involving those p processes.
	- this program also proceeds in log2p steps
	- from the Figure 1: the program starts with i-2 step and ends with all the data in P0
*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>

#define N 8
int main(int argc, char** argv)
{
	int rank, size, master = 0, buffer[N];
	MPI_Status status;

	printf("Scatter:\n");

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == master)
		for (int i = 0; i < N; i++)
			buffer[i] = i;

	int steps = log2(size);
	int mask = 0;

	for (int i = 1 << (steps - 1); i > 0; i >>= 1)
	{
		mask |= i;
		if ((mask & rank) == rank)
		{
			if ((rank ^ i) > rank)
				MPI_Send(&buffer[i], i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD);
			else
				MPI_Recv(&buffer[0], i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD, &status);
		}
	}

	printf("Process %d, data = [%d]\n", rank, buffer[0]);

	MPI_Barrier(MPI_COMM_WORLD);

	buffer[0] = rank;
	mask = size - 1;

	for (int i = 1; i < (1 << steps); i <<= 1)
	{
		if ((mask & rank) == rank)
		{
			if ((rank ^ i) < rank)
				MPI_Send(&buffer[0], i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD);
			else
				MPI_Recv(&buffer[i], i, MPI_INT, rank ^ i, 0, MPI_COMM_WORLD, &status);
		}

		mask ^= i;
	}

	if (rank == master)
	{
		printf("Gather: Process %d, data = [\t", rank);
		for (int i = 0; i < N; i++)
			printf("%d\t", buffer[i]);
		printf("]\n");
	}
		
	MPI_Finalize();
	return 0;
}