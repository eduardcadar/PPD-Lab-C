#include <iostream>
#include <fstream>
#include <mpi.h>
#include <string>
#include <algorithm>
#include <thread>
using namespace std;

string test = "1";
string nr1Path = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB_C\\Lab3\\Numar1test" + test + ".txt";
string nr2Path = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB_C\\Lab3\\Numar2test" + test + ".txt";
string rezPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB_C\\Lab3\\RezTest" + test + ".txt";
int varianta = 0;

ifstream fin1(nr1Path);
ifstream fin2(nr2Path);
ifstream finrez(rezPath);

int* varianta0(int n1, int n2)
{
	int a, b;
	int n = max(n1, n2);
	int* rez = new int[n + 1];
	int carry = 0;
	for (int i = 0; i < n; i++) {
		a = b = 0;
		if (i < n1)
			fin1 >> a;
		if (i < n2)
			fin2 >> b;
		int suma = a + b + carry;
		rez[i] = suma % 10;
		carry = suma / 10;
	}
	rez[n] = carry;
	return rez;
}

int* varianta1(int rank, int numProcs, int n1, int n2)
{
	int n = max(n1, n2);
	int* lengths = new int[numProcs - 1];
	int start = 0;
	int end = n / (numProcs - 1);
	int r = n % (numProcs - 1);
	for (int i = 0; i < numProcs - 1; i++) {
		if (r > 0) end++, r--;
		lengths[i] = end - start;
		start = end;
		end += n / (numProcs - 1);
	}

	if (rank == 0) {
		int* nr1 = new int[lengths[0]];
		int* nr2 = new int[lengths[0]];
		for (int p = 1; p < numProcs; p++) {
			for (int i = 0; i < lengths[p - 1]; i++) {
				nr1[i] = nr2[i] = 0;
				if (i < n1)
					fin1 >> nr1[i];
				if (i < n2)
					fin2 >> nr2[i];
			}
			MPI_Send(nr1, lengths[p - 1], MPI_INT, p, 111, MPI_COMM_WORLD);
			MPI_Send(nr2, lengths[p - 1], MPI_INT, p, 112, MPI_COMM_WORLD);
		}
		MPI_Status status;
		int* rez = new int[n + 1];
		int start = 0;
		for (int p = 1; p < numProcs; p++) {
			MPI_Recv(rez + start, lengths[p - 1], MPI_INT, p, 222, MPI_COMM_WORLD, &status);
			start += lengths[p - 1];
		}
		MPI_Recv(rez + n, 1, MPI_INT, numProcs - 1, 113, MPI_COMM_WORLD, &status);

		delete[] nr1;
		delete[] nr2;
		delete[] lengths;
		return rez;
	}
	else {
		int* nr1Proces = new int[lengths[rank - 1]];
		int* nr2Proces = new int[lengths[rank - 1]];
		int* rezProces = new int[lengths[rank - 1]];
		MPI_Status status1, status2, status3;
		MPI_Recv(nr1Proces, lengths[rank - 1], MPI_INT, 0, 111, MPI_COMM_WORLD, &status1);
		MPI_Recv(nr2Proces, lengths[rank - 1], MPI_INT, 0, 112, MPI_COMM_WORLD, &status2);
		int carry = 0;
		if (rank > 1)
			MPI_Recv(&carry, 1, MPI_INT, rank - 1, 113, MPI_COMM_WORLD, &status3);
		for (int i = 0; i < lengths[rank - 1]; i++) {
			int suma = nr1Proces[i] + nr2Proces[i] + carry;
			rezProces[i] = suma % 10;
			carry = suma / 10;
		}
		MPI_Send(rezProces, lengths[rank - 1], MPI_INT, 0, 222, MPI_COMM_WORLD);
		MPI_Send(&carry, 1, MPI_INT, (rank + 1) % numProcs, 113, MPI_COMM_WORLD);

		delete[] nr1Proces;
		delete[] nr2Proces;
		delete[] rezProces;
		delete[] lengths;
		return new int[0];
	}
}

int* varianta2(int rank, int numProcs, int n1, int n2)
{
	int n = max(n1, n2);
	int len = n / numProcs;
	if (n % numProcs != 0)
		len++;
	int nAlloc = len * numProcs;
	int* nr1 = new int[nAlloc];
	int* nr2 = new int[nAlloc];
	int* rez = new int[nAlloc + 1];
	int* nr1Proces = new int[len];
	int* nr2Proces = new int[len];
	int* rezProces = new int[len];
	if (rank == 0) {
		for (int i = 0; i < n; i++) {
			nr1[i] = 0;
			nr2[i] = 0;
			if (i < n1)
				fin1 >> nr1[i];
			if (i < n2)
				fin2 >> nr2[i];
		}
		for (int i = n1; i < nAlloc; i++)
			nr1[i] = 0;
		for (int i = n2; i < nAlloc; i++)
			nr2[i] = 0;
	}
	MPI_Scatter(nr1, len, MPI_INT, nr1Proces, len, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(nr2, len, MPI_INT, nr2Proces, len, MPI_INT, 0, MPI_COMM_WORLD);
	int carry = 0;
	MPI_Status status;
	if (rank > 0)
		MPI_Recv(&carry, 1, MPI_INT, rank - 1, 113, MPI_COMM_WORLD, &status);
	for (int i = 0; i < len; i++) {
		int suma = nr1Proces[i] + nr2Proces[i] + carry;
		rezProces[i] = suma % 10;
		carry = suma / 10;
	}
	if (rank < numProcs - 1)
		MPI_Send(&carry, 1, MPI_INT, rank + 1, 113, MPI_COMM_WORLD);
	else {
		rez[n] = carry;
		MPI_Send(rez + n, 1, MPI_INT, 0, 114, MPI_COMM_WORLD);
	}
	if (rank == 0)
		MPI_Recv(rez + n, 1, MPI_INT, numProcs - 1, 114, MPI_COMM_WORLD, &status);
	MPI_Gather(rezProces, len, MPI_INT, rez, len, MPI_INT, 0, MPI_COMM_WORLD);
	delete[] nr1;
	delete[] nr2;
	delete[] nr1Proces;
	delete[] nr2Proces;
	delete[] rezProces;
	if (rank == 0)
		return rez;
	return new int[0];
}

int* varianta3(int rank, int numProcs, int n1, int n2)
{
	int n = max(n1, n2);
	int* lengths = new int[numProcs - 1];
	int start = 0;
	int end = n / (numProcs - 1);
	int r = n % (numProcs - 1);
	for (int i = 0; i < numProcs - 1; i++) {
		if (r > 0) end++, r--;
		lengths[i] = end - start;
		start = end;
		end += n / (numProcs - 1);
	}
	if (rank == 0) {
		int* nr1 = new int[lengths[0]];
		int* nr2 = new int[lengths[0]];
		for (int p = 1; p < numProcs; p++) {
			for (int i = 0; i < lengths[p - 1]; i++) {
				nr1[i] = nr2[i] = 0;
				if (i < n1)
					fin1 >> nr1[i];
				if (i < n2)
					fin2 >> nr2[i];
			}
			MPI_Send(nr1, lengths[p - 1], MPI_INT, p, 111, MPI_COMM_WORLD);
			MPI_Send(nr2, lengths[p - 1], MPI_INT, p, 112, MPI_COMM_WORLD);
		}
		MPI_Status status;
		MPI_Request* requests = new MPI_Request[numProcs - 1];
		MPI_Request reqCarry;
		int* rez = new int[n + 1];
		int start = 0;
		for (int p = 1; p < numProcs; p++) {
			MPI_Irecv(rez + start, lengths[p - 1], MPI_INT, p, 222, MPI_COMM_WORLD, requests + p - 1);
			start += lengths[p - 1];
		}
		MPI_Irecv(rez + n, 1, MPI_INT, numProcs - 1, 113, MPI_COMM_WORLD, &reqCarry);
		for (int i = 0; i < numProcs - 1; i++)
			MPI_Wait(requests + i, &status);
		MPI_Wait(&reqCarry, &status);
		delete[] nr1;
		delete[] nr2;
		delete[] lengths;
		return rez;
	}
	else {
		int* nr1Proces = new int[lengths[rank - 1]];
		int* nr2Proces = new int[lengths[rank - 1]];
		int* rezProces = new int[lengths[rank - 1]];
		MPI_Status status1, status2, status3;
		MPI_Request req1, req2, reqCarry;
		MPI_Irecv(nr1Proces, lengths[rank - 1], MPI_INT, 0, 111, MPI_COMM_WORLD, &req1);
		MPI_Irecv(nr2Proces, lengths[rank - 1], MPI_INT, 0, 112, MPI_COMM_WORLD, &req2);
		int carry = 0;
		if (rank > 1)
			MPI_Irecv(&carry, 1, MPI_INT, rank - 1, 113, MPI_COMM_WORLD, &reqCarry);
		MPI_Wait(&req1, &status1);
		MPI_Wait(&req2, &status2);
		if (rank > 1)
			MPI_Wait(&reqCarry, &status3);
		for (int i = 0; i < lengths[rank - 1]; i++) {
			int suma = nr1Proces[i] + nr2Proces[i] + carry;
			rezProces[i] = suma % 10;
			carry = suma / 10;
		}
		MPI_Send(rezProces, lengths[rank - 1], MPI_INT, 0, 222, MPI_COMM_WORLD);
		MPI_Send(&carry, 1, MPI_INT, (rank + 1) % numProcs, 113, MPI_COMM_WORLD);
		delete[] nr1Proces;
		delete[] nr2Proces;
		delete[] rezProces;
		delete[] lengths;
		return new int[0];
	}
}

bool checkResult(int* rez, int n)
{
	int a;
	for (int i = 0; i < n; i++) {
		finrez >> a;
		if (rez[i] != a)
			return false;
	}
	return true;
}

void generateFile(string filePath, int n)
{
	ofstream g(filePath);
	g << n << '\n';
	srand(time(0));
	for (int i = 0; i < n; i++) {
		g << rand() % 10 << ' ';
	}
	g.close();
}

int main(int argc, char* argv[])
{
	int n1, n2;
	fin1 >> n1;
	fin2 >> n2;
	if (varianta == 0) {
		auto start = chrono::steady_clock::now();
		int* rez = varianta0(n1, n2);
		auto finish = chrono::steady_clock::now();
		auto time = chrono::duration <double, milli>(finish - start).count();
		if (!checkResult(rez, max(n1, n2) + 1))
			cout << "WRONG RESULT";
		else
			cout << time;
		delete[] rez;
		return 0;
	}
	else {
		int rank, numProcs;
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
		int* rez = new int[0];
		auto start = chrono::steady_clock::now();
		if (varianta == 1)
			rez = varianta1(rank, numProcs, n1, n2);
		if (varianta == 2)
			rez = varianta2(rank, numProcs, n1, n2);
		if (varianta == 3)
			rez = varianta3(rank, numProcs, n1, n2);
		auto finish = chrono::steady_clock::now();
		auto time = chrono::duration <double, milli>(finish - start).count();
		if (rank == 0) {
			if (!checkResult(rez, max(n1, n2) + 1))
				cout << "WRONG RESULT";
			else
				cout << time;
		}
		delete[] rez;
		MPI_Finalize();
	}
	fin1.close();
	fin2.close();
	finrez.close();
	return 0;
}