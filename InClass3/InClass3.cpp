#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
#define SIZE 100
int a[SIZE], b[SIZE], c[SIZE];

int main(int argc, char* argv[])
{
	int rank, numProcs;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

	int len = SIZE / numProcs;
	int* a2 = new int[len];
	int* b2 = new int[len];
	int* c2 = new int[len];

	if (rank == 0) {
		cout << "Process rank: " << rank << ", noProcs: " << numProcs << '\n';
		for (int i = 0; i < SIZE; i++) {
			a[i] = i;
			b[i] = i;
		}
	}

	MPI_Scatter(a, len, MPI_INT, a2, len, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, len, MPI_INT, b2, len, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < len; i++)
		c2[i] = a2[i] + b2[i];

	MPI_Gather(c2, len, MPI_INT, c, len, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (int i = 0; i < SIZE; i++)
			cout << c[i] << ' ';
	}

	delete[] a2;
	delete[] b2;
	delete[] c2;

	MPI_Finalize();
	return 0;
}

//int rank, numProcs;
//MPI_Init(&argc, &argv);
//MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

//int len = SIZE / numProcs;
//if (rank == 0) {
//	cout << "Process rank: " << rank << ", noProcs: " << numProcs << '\n';
//	int a[SIZE], b[SIZE];
//	for (int i = 0; i < SIZE; i++) {
//		a[i] = i;
//		b[i] = i;
//	}
//	for (int i = 1; i < numProcs; i++) {
//		MPI_Send(a + len * i, len, MPI_INT, i, 111, MPI_COMM_WORLD);
//		MPI_Send(b + len * i, len, MPI_INT, i, 111, MPI_COMM_WORLD);
//	}
//	for (int i = 0; i < len; i++)
//		c[i] = a[i] + b[i];
//	MPI_Status status;
//	for (int i = 1; i < numProcs; i++) {
//		MPI_Recv(c + i * len, len, MPI_INT, i, 12345, MPI_COMM_WORLD, &status);
//	}
//
//	cout << "Process " << rank << ": ";
//	for (int i = 0; i < SIZE; i++)
//		cout << c[i] << ' ';
//}
//else {
//	MPI_Status statusA, statusB;
//	MPI_Recv(a, len, MPI_INT, 0, 111, MPI_COMM_WORLD, &statusA);
//	MPI_Recv(b, len, MPI_INT, 0, 111, MPI_COMM_WORLD, &statusB);
//	for (int i = 0; i < len; i++)
//		c[i] = a[i] + b[i];
//	MPI_Send(c, len, MPI_INT, 0, 12345, MPI_COMM_WORLD);
//}

//MPI_Finalize();
