#include "Common.h"

double Common::CalculateValue(int i, int j, Matrix& matrix, Matrix& windowMatrix) {
	double output = 0;
	for (int k = -windowMatrix.n / 2; k <= windowMatrix.n / 2; k++) {
		for (int l = -windowMatrix.m / 2; l <= windowMatrix.m / 2; l++) {
			int a = i + k;
			int b = j + l;
			if (a < 0) a = 0;
			if (b < 0) b = 0;
			if (a >= matrix.n) a = matrix.n - 1;
			if (b >= matrix.m) b = matrix.m - 1;

			output +=
				matrix.values[a][b] * windowMatrix.values[k + windowMatrix.n / 2][l + windowMatrix.m / 2];
		}
	}
	return output;
}