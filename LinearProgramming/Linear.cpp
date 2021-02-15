#include "Linear.h"

Matrix::Matrix(size_t n, size_t m = 0) : n{ n }, m{ m == 0 ? n : m } {
	matrix = new double* [n];
	for (size_t i = 0; i < n; ++i) {
		matrix[i] = new double[m];
		for (size_t j = 0; j < m; ++j) {
			matrix[i][j] = 0;
		}
	}
}

Matrix::Vector::Vector(double* vector) : vector{ vector } {}

double& Matrix::Vector::operator[](size_t index) {
	return vector[index];
}

Matrix::Vector Matrix::operator[](size_t index) {
	return Vector(matrix[index]);
}

void Matrix::print() const {
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

Matrix::~Matrix() {
	for (size_t i = 0; i < n; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

Linear::Linear(std::vector<double>& function, Limitations& limitations, std::vector<bool>& vars_sign) 
	: A(limitations.limitations.size(), vars_in_canonical(function, limitations, vars_sign)),
	b(limitations.limitations.size()),
	objective_function(vars_in_canonical(function, limitations, vars_sign)),
	dual_program(nullptr) {

	for (size_t i = 0; i < b.size(); ++i) {
		b[i] = limitations.limitations[i].first[function.size()];
	}

	size_t m = 0;
	for (size_t i = 0; i < function.size(); ++i, ++m) {
		if (vars_sign[i]) {
			objective_function[m] = function[i];
		}
		else {
			objective_function[m++] = function[i];
			objective_function[m] = -function[i];
		}
	}

	for (size_t i = 0, n = m; i < limitations.limitations.size(); ++i) {

		for (size_t k = 0, j = 0; k < limitations.limitations[i].first.size() - 1; ++k, ++j) {
			if (vars_sign[k]) {
				A[i][j] = limitations.limitations[i].first[k];
			}
			else {
				A[i][j++] = limitations.limitations[i].first[k];
				A[i][j] = -limitations.limitations[i].first[k];
			}
		}

		if (limitations.limitations[i].second == LT::LT_GT) {
			A[i][n++] = -1;
		}
		if (limitations.limitations[i].second == LT::LT_LE) {
			A[i][n++] = 1;
		}

	}

	//Для проверки
	std::cout << "c:\n";
	for (auto elem : objective_function) {
		std::cout << elem << ' ';
	}
	std::cout << "\nA:\n";
	A.print();
	std::cout << "\nb:\n";
	for (auto elem : b) {
		std::cout << elem << ' ';
	}
}

Linear* Linear::get_dual_program() {
	return dual_program;
}

size_t Linear::vars_in_canonical(std::vector<double>& function, Limitations& limitations, std::vector<bool>& vars_sign) {
	size_t vars_num = function.size();
	for (auto elem : vars_sign) {
		if (!elem) {
			++vars_num;
		}
	}
	for (auto vec : limitations.limitations) {
		if (vec.second != LT::LT_EQ) {
			++vars_num;
		}
	}
	return vars_num;
}