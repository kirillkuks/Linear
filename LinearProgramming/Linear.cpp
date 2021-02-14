#include "Linear.h"

Matrix::Matrix(size_t n, size_t m = 0) : n{ n }, m{ m == 0 ? n : m } {
	matrix = new double* [n];
	for (size_t i = 0; i < n; ++i) {
		matrix[i] = new double[m];
	}
}

Matrix::Vector::Vector(double* vector) : vector{ vector } {}

double& Matrix::Vector::operator[](size_t index) {
	return vector[index];
}

Matrix::Vector&& Matrix::operator[](size_t index) {
	return std::move(Vector(matrix[index]));
}

Matrix::~Matrix() {
	for (size_t i = 0; i < n; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;
}