#pragma once

class Matrix {
public:
	Matrix() {
		n = m = 0;
		values = 0;
	}

	int n, m;
	double** values;
};
