#include "Secvential.h"

Matrix Secvential::Solve(Matrix borderedMatrix, Matrix windowMatrix) {
	Matrix outputMatrix = Matrix();
	outputMatrix.n = borderedMatrix.n - 2;
	outputMatrix.m = borderedMatrix.m - 2;
	outputMatrix.values = new double* [outputMatrix.n];
	for (int i = 0; i < outputMatrix.n; i++)
		outputMatrix.values[i] = new double[outputMatrix.m];

	for (int i = 0; i < outputMatrix.n; i++) {
		for (int j = 0; j < outputMatrix.m; j++) {
			outputMatrix.values[i][j] = 0.0;
			for (int k = -windowMatrix.n / 2; k <= windowMatrix.n / 2; k++) {
				for (int l = -windowMatrix.m / 2; l <= windowMatrix.m / 2; l++)
					outputMatrix.values[i][j] +=
						borderedMatrix.values[i + 1 + k][j + 1 + l] * windowMatrix.values[k + windowMatrix.n / 2][l + windowMatrix.m / 2];
			}
		}
	}

	return outputMatrix;
}