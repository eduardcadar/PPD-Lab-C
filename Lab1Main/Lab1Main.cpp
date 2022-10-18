#include "../Utils/FileUtils.h"
#include "../Utils/Matrix.h"
#include "../Utils/MatrixUtils.h"
#include "Secvential.h"
#include <iostream>
#include "Parallel.h"
using std::cout;

int main()
{
    string matrixPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\matrixEx.txt";
    string windowMatrixPath = "C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\windowMatrixEx.txt";
    const int no_threads = 5;

    Matrix matrix = FileUtils::ReadFileAsMatrix(matrixPath);
    Matrix windowMatrix = FileUtils::ReadFileAsMatrix(windowMatrixPath);

    Secvential secventialSolver = Secvential();
    Parallel parallelSolver = Parallel();

    Matrix outputMatrixSeq = secventialSolver.Solve(matrix, windowMatrix);

    Matrix outputMatrixPar = parallelSolver.Solve(matrix, windowMatrix, no_threads);
 
    MatrixUtils::PrintMatrix(outputMatrixSeq);
    cout << '\n';
    MatrixUtils::PrintMatrix(outputMatrixPar);
    cout << '\n';

    if (MatrixUtils::SameElements(outputMatrixSeq, outputMatrixPar))
        cout << "Same elements";
    else
        cout << "Not same elements";
}
