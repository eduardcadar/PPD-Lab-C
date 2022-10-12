#include "FileUtils.h"
#include <fstream>
using std::ifstream;

bool FileUtils::CompareFiles(string filePath1, string filePath2) {
	ifstream f1(filePath1);
	ifstream f2(filePath2);

	while (f1.get() != f2.get()) {}
	bool b1 = f1.eof(), b2 = f2.eof();
	f1.close();
	f2.close();
	return b1 == b2 == true;
}

Matrix FileUtils::ReadFileAsMatrix(string filePath) {
	ifstream fin(filePath);

	int n, m;
	fin >> n >> m;
	Matrix matrix = Matrix();
	matrix.n = n;
	matrix.m = m;

	double** matrixValues = new double* [n];
	for (int i = 0; i < n; i++)
		matrixValues[i] = new double[m];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			fin >> matrixValues[i][j];

	matrix.values = matrixValues;
	return matrix;
}
