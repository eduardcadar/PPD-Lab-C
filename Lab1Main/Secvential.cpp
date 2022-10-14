#include "Secvential.h"

Matrix Secvential::Solve(Matrix matrix, Matrix windowMatrix) {
	Matrix outputMatrix = Matrix();
	outputMatrix.n = matrix.n;
	outputMatrix.m = matrix.m;
	outputMatrix.values = new double* [outputMatrix.n];
	for (int i = 0; i < outputMatrix.n; i++)
		outputMatrix.values[i] = new double[outputMatrix.m];

	for (int i = 0; i < outputMatrix.n; i++) {
		for (int j = 0; j < outputMatrix.m; j++) {
			outputMatrix.values[i][j] = 0.0;
			for (int k = -windowMatrix.n / 2; k <= windowMatrix.n / 2; k++) {
				for (int l = -windowMatrix.m / 2; l <= windowMatrix.m / 2; l++) {
					int a = i + k;
					int b = j + l;
					if (a < 0) a = 0;
					if (b < 0) b = 0;
					if (a >= matrix.n) a = matrix.n - 1;
					if (b >= matrix.m) b = matrix.m - 1;

					outputMatrix.values[i][j] +=
						matrix.values[a][b] * windowMatrix.values[k + windowMatrix.n / 2][l + windowMatrix.m / 2];
				}
			}
		}
	}

	return outputMatrix;
}