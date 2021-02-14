#pragma once
#include <iostream>
#include <vector>

struct Matrix {
private:
	struct Vector;
public:
	Matrix(size_t, size_t);

	Vector operator[](size_t);

	~Matrix();
private:
	struct Vector {
		Vector(double*);
		double& operator[](size_t);
	private:
		double* vector;
	};

	double** matrix;
	size_t n, m;
};

struct Linear {
	void create_dual_program();
	Linear* get_dual_program();
private:
	Matrix matrix;
	std::vector<double> objective_function;

	Linear* dual_program;
};