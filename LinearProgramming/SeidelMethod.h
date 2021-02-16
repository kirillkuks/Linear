#pragma once
#include <cmath>
#include "Linear.h"

struct Matrix;

struct SeidelMethod {
public:
	static std::vector<double> solve(Matrix&, std::vector<double>&);
private:
	static Matrix multy(Matrix&, Matrix&);
	static std::vector<double> b_create(Matrix&, std::vector<double>&);
	static double norm(std::vector<double>&);
};