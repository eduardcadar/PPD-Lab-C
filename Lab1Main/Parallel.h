#pragma once
#include "../Utils/Matrix.h"
class Parallel {
public:
	Matrix Solve(Matrix matrix, Matrix windowMatrix, int no_threads);
};

