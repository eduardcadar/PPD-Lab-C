#include "../Utils/FileUtils.h"
#include "../Utils/Matrix.h"
#include "../Utils/MatrixUtils.h"
#include "Secvential.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include "Parallel.h"
using namespace std;
#define NMAX 10
#define MMAX 10
#define nMAX 3
#define mMAX 3

double matrix[NMAX][MMAX], windowMatrix[nMAX][mMAX], outputMatrix[NMAX][MMAX];

void ReadFileAsMatrixStatic(string filePath) {
    ifstream fin(filePath);

    int n, m;
    fin >> n >> m;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fin >> matrix[i][j];
}

void ReadFileAsWindowMatrixStatic(string filePath) {
    ifstream fin(filePath);

    int n, m;
    fin >> n >> m;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fin >> windowMatrix[i][j];
}

double CalculateValueStatic(int i, int j) {
    double output = 0;
    for (int k = -nMAX / 2; k <= nMAX / 2; k++) {
        for (int l = -mMAX / 2; l <= mMAX / 2; l++) {
            int a = i + k;
            int b = j + l;
            if (a < 0) a = 0;
            if (b < 0) b = 0;
            if (a >= NMAX) a = NMAX - 1;
            if (b >= MMAX) b = MMAX - 1;

            output +=
                matrix[a][b] * windowMatrix[k + nMAX / 2][l + mMAX / 2];
        }
    }
    return output;
}

void SeqSolveStatic() {
    for (int i = 0; i < NMAX; i++)
        for (int j = 0; j < MMAX; j++)
            outputMatrix[i][j] = CalculateValueStatic(i, j);
}

void ParSolveStatic(int no_threads)
{
    thread* threads = new thread[no_threads];
    int start, end, r;
    start = 0;
    int size = NMAX;
    end = size / no_threads;
    r = size % no_threads;
    for (int i = 0; i < no_threads; i++) {
        if (r > 0)
            end++, r--;
        threads[i] = thread([&](int start, int end) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < MMAX; j++)
                    outputMatrix[i][j] = CalculateValueStatic(i, j);
            }}, start, end);
        start = end;
        end += size / no_threads;
    }
    for (int i = 0; i < no_threads; i++)
        threads[i].join();
}

int main(int argc, char* argv[])
{
    string matrixPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\matrix1.txt";
    string windowMatrixPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\windowMatrix1.txt";
    int no_threads = 4;
    no_threads = stoi(argv[1]);
    
    bool secv = false;

    //Matrix matrix = FileUtils::ReadFileAsMatrix(matrixPath);
    //Matrix windowMatrix = FileUtils::ReadFileAsMatrix(windowMatrixPath);

    ReadFileAsMatrixStatic(matrixPath);
    ReadFileAsWindowMatrixStatic(windowMatrixPath);

    //Secvential secventialSolver = Secvential();
    Parallel parallelSolver = Parallel();

    if (secv) {
        auto start = chrono::steady_clock::now();
        //Matrix outputMatrixSeq = secventialSolver.Solve(matrix, windowMatrix);
        SeqSolveStatic();
        auto finish = chrono::steady_clock::now();
        auto tSeq = chrono::duration <double, milli>(finish - start).count();
        //cout << "Time for seq: " << tSeq << " ms\n";
        cout << tSeq;
    }
    else {
        auto start = chrono::steady_clock::now();
        //Matrix outputMatrixSeq = parallelSolver.Solve(matrix, windowMatrix, no_threads);
        ParSolveStatic(no_threads);
        auto finish = chrono::steady_clock::now();
        auto tPar = chrono::duration <double, milli>(finish - start).count();
        //cout << "Time for par: " << tPar << " ms\n";
        cout << tPar;
    }

    //MatrixUtils::PrintMatrix(outputMatrixSeq);
    //cout << '\n';
    //MatrixUtils::PrintMatrix(outputMatrixPar);
    //cout << '\n';

    //if (MatrixUtils::SameElements(outputMatrixSeq, outputMatrixPar))
    //    cout << "Same elements";
    //else
    //    cout << "Not same elements";
}
