#include "Parallel.h"
#include <thread>
#include "Common.h"
using namespace std;

Matrix Parallel::Solve(Matrix& matrix, Matrix& windowMatrix, int no_threads)
{
	Matrix outputMatrix = Matrix();
	outputMatrix.n = matrix.n;
	outputMatrix.m = matrix.m;
	outputMatrix.values = new double* [outputMatrix.n];
	for (int i = 0; i < outputMatrix.n; i++)
		outputMatrix.values[i] = new double[outputMatrix.m];

	thread* threads = new thread[no_threads];
	int start, end, r;
	start = 0;
	int size = matrix.n;
	end = size / no_threads;
	r = size % no_threads;
	for (int i = 0; i < no_threads; i++) {
		if (r > 0)
			end++, r--;
		threads[i] = thread([&](Matrix& matrix, Matrix& windowMatrix, Matrix& outputMatrix, int start, int end) {
			for (int i = start; i < end; i++) {
				for (int j = 0; j < outputMatrix.m; j++)
					outputMatrix.values[i][j] = Common::CalculateValue(i, j, matrix, windowMatrix);
			}}, ref(matrix), ref(windowMatrix), ref(outputMatrix), start, end);
		start = end;
		end += size / no_threads;
	}
	for (int i = 0; i < no_threads; i++)
		threads[i].join();

	return outputMatrix;
}
