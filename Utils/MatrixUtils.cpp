#include "MatrixUtils.h"
#include <iostream>
#include <iterator>
#include <algorithm>
using std::cout;

Matrix MatrixUtils::AddBorderToMatrix(Matrix matrix) {
	Matrix borderedMatrix = Matrix();
    borderedMatrix.n = matrix.n + 2;
    borderedMatrix.m = matrix.m + 2;
    borderedMatrix.values = new double* [borderedMatrix.n];
    for (int i = 0; i < borderedMatrix.n; i++)
        borderedMatrix.values[i] = new double[borderedMatrix.m];

    for (int i = 0; i < matrix.n; i++)
        std::copy(matrix.values[i], matrix.values[i] + matrix.m, &borderedMatrix.values[i + 1][1]);
    std::copy(matrix.values[0], matrix.values[0] + matrix.m, &borderedMatrix.values[0][1]);
    std::copy(matrix.values[matrix.n - 1], matrix.values[matrix.n - 1] + matrix.m, &borderedMatrix.values[borderedMatrix.n - 1][1]);

    for (int i = 0; i < borderedMatrix.n; i++) {
        borderedMatrix.values[i][0] = borderedMatrix.values[i][1];
        borderedMatrix.values[i][borderedMatrix.m - 1] = borderedMatrix.values[i][borderedMatrix.m - 2];
    }

	return borderedMatrix;
}

void MatrixUtils::PrintMatrix(Matrix matrix) {
    cout << matrix.n << ' ' << matrix.m << '\n';
    for (int i = 0; i < matrix.n; i++) {
        for (int j = 0; j < matrix.m; j++)
            cout << matrix.values[i][j] << ' ';
        cout << '\n';
    }
}
