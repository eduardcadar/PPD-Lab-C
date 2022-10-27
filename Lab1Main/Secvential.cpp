#include "Secvential.h"
#include "Common.h"

Matrix Secvential::Solve(Matrix& matrix, Matrix& windowMatrix) {
	Matrix outputMatrix = Matrix();
	outputMatrix.n = matrix.n;
	outputMatrix.m = matrix.m;
	outputMatrix.values = new double* [outputMatrix.n];
	for (int i = 0; i < outputMatrix.n; i++)
		outputMatrix.values[i] = new double[outputMatrix.m];

	for (int i = 0; i < outputMatrix.n; i++)
		for (int j = 0; j < outputMatrix.m; j++)
			outputMatrix.values[i][j] = Common::CalculateValue(i, j, matrix, windowMatrix);

	return outputMatrix;
}