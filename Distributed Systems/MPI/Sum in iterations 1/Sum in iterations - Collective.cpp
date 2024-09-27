/*
	MPI program for calculating sum described as:
	- for (i=0;i<N;i++)
		for (j=0;j<N;j++)
			sum+=i+j;
	- with an even cyclic distribution of work among p processes.
	- this means that if k is the process identifier, then process Pk (0 ≤ k ≤ p-1) should perform the summation for k, k+p, k+2p, ..., k+N²-p sequentially in the program execution.
	- assume that N is greater than the number of processes p and that N is divisible by p.
	- the result of the program should be displayed in the process containing the largest number of summands that are prime numbers. 
	- the use of indexed variables is not allowed. 
	- solve the task: Using collective operations 
*/

#include <mpi.h>
#include <stdio.h>
#include <malloc.h>

#define N 8

int main(int argc, char** argv)
{
	int rank, size, master = 0;
	int local_sum = 0, sum = 0;

	int num_prime = 0;
	bool prime;

	int main_array[N*N];
	int* local_array = nullptr;

	//Structure for finding the most prime numbers
	struct { int val; int rank; } max, gmax;

	// Used to check if the result is correct
	int realsum = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			realsum += i + j;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int local_size = N * N / size;

	local_array = (int*)malloc(sizeof(int) * local_size);

	if (rank == master)
	{
		for (int i = 0; i < N*N; i++)
			main_array[i] = i;
	}

	for (int i = 0; i < local_size; i++)
	{
		MPI_Scatter(&main_array[i * size + rank], 1, MPI_INT, &local_array[i], 1, MPI_INT, master, MPI_COMM_WORLD);
	}

	for (int i = 0; i < local_size; i++)
	{
		local_sum += local_array[i] / N + local_array[i] % N;

		prime = true;
		int j = 2;
		int local_i = local_array[i] / N;
		int local_j = local_array[i] % N;

		while (prime && j <= local_i / 2) //need to check only till one half of the number
		{
			if (local_i % j == 0)
				prime = false;
			j++;
		}
		if (prime)
			num_prime++;

		prime = true;
		j = 2;
		while (prime && j <= local_j / 2)
		{
			if (local_j % j == 0)
				prime = false;
			j++;
		}
		if (prime)
			num_prime++;
	}

	max.val = num_prime;
	max.rank = rank;

	MPI_Reduce(&max, &gmax, 1, MPI_2INT, MPI_MAXLOC, master, MPI_COMM_WORLD);
	MPI_Bcast(&gmax, 1, MPI_2INT, master, MPI_COMM_WORLD);
	MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, gmax.rank, MPI_COMM_WORLD);

	if (rank == gmax.rank)
	{
		printf("Sum = %d is in process with rank %d\n", sum, rank);
		printf("Real sum = %d\n", realsum);

		printf("Number of prime numbers in P%d is: %d\n", rank, gmax.val);
	}

	free(local_array);

	MPI_Finalize();
	return 0;
}