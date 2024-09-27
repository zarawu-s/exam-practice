/*
	MPI program for calculating standard deviation of a set of N numbers, using the formula in Figure 1.
	- xi is an element of the set, and x- is the mean of all N numbers
	- in process P0, the initialization of N numbers is performed
	- after that, an equal number of numbers is distributed to all processes (N is divisible by the total bumber of processes)
	- after that, the mean of all the numbers is calculated
	- after that, the calculation of the standard deviation of the numbers
	- all processes participate in the calculations and the result is displayed in process P0
	- solve the task using only collective operations
*/

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#define N 8

int main(int argc, char** argv)
{
	int rank, size, master = 0;

	int array_x[N];
	double mean, sum_dev;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int local_size = N / size;
	int* local_array = (int*)malloc(sizeof(int) * local_size);

	if (rank == master)
	{
		for (int i = 0; i < N; i++)
			array_x[i] = i * 2;
	}

	for (int i = 0; i < size; i++)
	{
		MPI_Scatter(&array_x[0], local_size, MPI_INT, &local_array[0], local_size, MPI_INT, master, MPI_COMM_WORLD);
	}

	double sum_mean, local_sum_mean = 0.0;

	for (int i = 0; i < local_size; i++)
		local_sum_mean += local_array[i];

	MPI_Reduce(&local_sum_mean, &sum_mean, size, MPI_DOUBLE, MPI_SUM, master, MPI_COMM_WORLD);
	MPI_Bcast(&sum_mean, 1, MPI_DOUBLE, master, MPI_COMM_WORLD);

	mean = (double)sum_mean / (double)N;

	double local_sum_dev = 0;
	for (int i = 0; i < local_size; i++)
	{
		local_sum_dev += pow((local_array[i] - mean), 2);
	}

	MPI_Reduce(&local_sum_dev, &sum_dev, size, MPI_DOUBLE, MPI_SUM, master, MPI_COMM_WORLD);


	if (rank == master)
	{
		double standard_deviation = sqrt((1.0 / (double)(N - 1)) * sum_dev);

		printf("Standard deviation of array:\t{\t");
		for (int i = 0; i < N; i++)
			printf("%d\t", array_x[i]);
		printf("}\t is: %f", standard_deviation);
	}
	
	free(local_array);

	MPI_Finalize();
	return 0;
}