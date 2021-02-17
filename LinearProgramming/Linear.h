#pragma once
#include <iostream>
#include <vector>
#include <limits>
#include "Parser.h"
#include "SeidelMethod.h"

struct SeidelMethod;
struct Limitations;

#define N 5

struct Matrix {
private:
	struct Vector;
public:
	Matrix(size_t, size_t);
	Matrix(Matrix const&);

	Matrix& operator=(Matrix);
	Vector operator[](size_t);

	size_t get_n() const;
	size_t get_m() const;
	Matrix get_transposed() const;

	void print() const;

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

enum TT {
	TT_MIN = 0,
	TT_MAX
};

struct Linear {
public:
	Linear(std::vector<double>&, Limitations&, std::vector<bool>&);

	std::vector<double> solve_task();

	void create_dual_program();
	Linear* get_dual_program();
private:
	static size_t vars_in_canonical(std::vector<double>&, Limitations&, std::vector<bool>&);
	static bool next_combination(std::vector<size_t>&, size_t);

	Linear(std::vector<double>&, Limitations&, std::vector<bool>&, Linear*);
	void to_canonical(std::vector<double>&, Limitations&, std::vector<bool>&);
	void to_dual(std::vector<double>&, Limitations&, std::vector<bool>&);
	
	bool is_linear_independence(Matrix&);
	double calculate_objective(std::vector<double>&);
	Matrix sub_matrix(std::vector<size_t>&);
	double determinant(Matrix);
	bool in_allowable_area(std::vector<double>&);
	std::vector<double> back_to_original_vars(std::vector<double>&);

	size_t original_dimension;

	Matrix A;
	std::vector<double> b;
	std::vector<double> objective_function;
	TT task_type;

	std::vector<std::pair<size_t, size_t>> original_vars;

	Linear* dual_program;
};