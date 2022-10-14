#include "../Utils/FileUtils.h"
#include "../Utils/Matrix.h"
#include "../Utils/MatrixUtils.h"
#include "Secvential.h"
#include <iostream>
using std::cout;

int main()
{
    Matrix matrix = FileUtils::ReadFileAsMatrix("C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\matrix1.txt");
    Matrix windowMatrix = FileUtils::ReadFileAsMatrix("C:\\facultate\\Semestrul 5\\PPD\\PPD_LAB\\Lab1\\src\\windowMatrix1.txt");

    Secvential secventialSolver = Secvential();
    Matrix outputMatrix = secventialSolver.Solve(matrix, windowMatrix);
 
    MatrixUtils::PrintMatrix(outputMatrix);
}
