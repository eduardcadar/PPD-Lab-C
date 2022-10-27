#pragma once
#include "Matrix.h"

class MatrixUtils {
public:
	static Matrix AddBorderToMatrix(Matrix& matrix);
	static void PrintMatrix(Matrix& matrix);
	static bool SameElements(Matrix& matrix1, Matrix& matrix2);
};