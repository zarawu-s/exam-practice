/*
	MPI program where each process generates the sum of integers using p = n processes.
	- p is the number of processes and n is a power of two
	- processes are arranged in a hypercube as shown in Figure 1 
	- after the program is executed, process Pk​ (k=0,p−1) should contain and display the sum of the first n numbers
	- the algorithm proceeds in log⁡2p steps
	- the addition process per step is shown in Figure 1, in parentheses are the values that each process adds in each step
	- in every step, the process with rank k exchanges its data with process 1−k xor 2^i (where i denotes the step)

*/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define n 16

int main(int argc, char** argv)
{
	int rank, size, master = 0;

	int broj, buff[n], pom, sum = 0;

	for (int i = 0; i < n; i++)
	{
		buff[i] = 0;
	}

	MPI_Status status;
	MPI_Request request;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	buff[rank] = rank;

	int steps = log2(size);
	int bar = 0;

	int temp;
	int mask = 7;


	for (int i = 1; i < (1 << steps); i <<= 1)
	{
		//Ranks of processes are different for sending/receiving
		MPI_Send(&buff[rank & mask], (int)pow((double)2, (double)bar), MPI_INT, rank ^ i, 0, MPI_COMM_WORLD);
		MPI_Recv(&buff[(rank & mask) ^ i], (int)pow((double)2, (double)bar), MPI_INT, rank ^ i, 0, MPI_COMM_WORLD, &status);

		/*if (rank == master)
			printf("ili mask: %d, maska za prijem: %d, maska za slanje: %d\n", ilimask, maskprijem, maskslanje);
		*/

		bar = i;
		mask -= i;
	}


	for (int j = 0; j < n; j++)
	{
		printf("P%d: buff[%d] = %d\n", rank, j, buff[j]);
		sum += buff[j];
	}

	printf("rank = %d, sum = %d \n ", rank, sum);

	MPI_Finalize();
	return 0;

}

