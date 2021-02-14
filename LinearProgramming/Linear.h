#pragma once
#include <vector>

struct Matrix {
private:
	struct Vector;
public:
	Matrix(size_t, size_t);

	Vector&& operator[](size_t);

	~Matrix();
private:
	struct Vector {
		Vector(int*);
		int& operator[](size_t);
	private:
		int* vector;
	};

	int** matrix;
	size_t n, m;
};

struct Linear {
private:
	Matrix matrix;
	int size;
};