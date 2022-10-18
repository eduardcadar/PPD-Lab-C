#pragma once

class Matrix {
public:
	int n, m;
	double** values;

	Matrix() {
		n = m = 0;
		values = 0;
	}
};
