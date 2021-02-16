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

Matrix::Matrix(Matrix const& other) : Matrix(other.n, other.m) {
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			matrix[i][j] = other.matrix[i][j];
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

size_t Matrix::get_n() const {
	return n;
}

size_t Matrix::get_m() const {
	return m;
}

Matrix Matrix::get_transposed() const {
	Matrix AT(m, n);
	for (size_t i = 0; i < m; ++i) {
		for (size_t j = 0; j < n; ++j) {
			AT[i][j] = matrix[j][i];
		}
	}
	return AT;
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
	original_dimension(function.size()),
	original_vars(function.size()),
	dual_program(nullptr) {

	for (size_t i = 0; i < b.size(); ++i) {
		b[i] = limitations.limitations[i].first[function.size()];
	}

	size_t m = 0;
	for (size_t i = 0; i < function.size(); ++i, ++m) {
		if (vars_sign[i]) {
			objective_function[m] = function[i];
			original_vars[i] = {m, m};
		}
		else {
			objective_function[m++] = function[i];
			objective_function[m] = -function[i];
			original_vars[i] = { m - 1, m };
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

double Linear::calculate_objective(std::vector<double>& x) {
	double res = 0;
	for (size_t i = 0; i < objective_function.size(); ++i) {
		res += objective_function[i] * x[i];
	}
	return res;
}

bool Linear::next_combination(std::vector<size_t>& vec, size_t n) {
	size_t k = vec.size();
	int i;
	for (i = k - 1; i >= 0 && vec[i] == n - 1; --i, --n) {}
	if (i < 0) {
		return false;
	}
	++vec[i];
	for (size_t j = i + 1; j < k; ++j) {
		vec[j] = vec[j - 1] + 1;
	}
	return true;
}

double Linear::determinant(Matrix matrix) {
	size_t size = b.size();
	double det = 1;
	for (size_t i = 0; i < size; ++i) {
		size_t k = i;
		for (size_t j = i + 1; j < size; ++j) {
			if (std::abs(matrix[j][i]) > std::abs(matrix[k][i])) {
				k = j;
			}
		}
		if (std::abs(matrix[k][i]) < 1E-5) {
			return 0;
		}
		for (size_t j = 0; j < size; ++j) {
			std::swap(matrix[i][j], matrix[k][j]);
		}
		if (i != k) {
			det = -det;
		}
		det *= matrix[i][i];
		for (size_t j = i + 1; j < size; ++j) {
			matrix[i][j] /= matrix[i][i];
		}
		for (size_t j = 0; j < size; ++j) {
			if (j != i && std::abs(matrix[j][i]) > 1E-5) {
				for (size_t k = i + 1; k < size; ++k) {
					matrix[j][k] -= matrix[i][k] * matrix[j][i];
				}
			}
		}
	}
	return det;
}

bool Linear::is_linear_independence(Matrix& submatrix) {
	return determinant(submatrix) != 0;
}

bool Linear::in_allowable_area(std::vector<double>& vec) {
	for (auto elem : vec) {
		if (elem < 0) {
			return false;
		}
	}
	return true;
}

Matrix Linear::sub_matrix(std::vector<size_t>& vec) {
	size_t size = vec.size();
	Matrix submatrix(size, size);
	for (size_t j = 0; j < size; ++j) {
		for (size_t i = 0; i < size; ++i) {
			submatrix[i][j] = A[i][vec[j]];
		}
	}
	return submatrix;
}

std::vector<double> Linear::back_to_original_vars(std::vector<double>& x) {
	std::vector<double> vars(original_dimension);
	for (size_t i = 0; i < original_dimension; ++i) {
		if (original_vars[i].first == original_vars[i].second) {
			vars[i] = x[original_vars[i].first];
		}
		else {
			vars[i] = x[original_vars[i].first] - x[original_vars[i].second];
		}
	}
	return vars;
}

std::vector<double> Linear::solve_task() {
	size_t canonical_dimension = objective_function.size();
	std::vector<double> optimal(canonical_dimension);
	double min = std::numeric_limits<double>::infinity();

	std::vector<size_t> vectors_in_basis(b.size());
	for (size_t i = 0; i < vectors_in_basis.size(); ++i) {
		vectors_in_basis[i] = i;
	}

	do {
		Matrix submatrix = sub_matrix(vectors_in_basis);
		if (is_linear_independence(submatrix)) {
			std::vector<double> x(canonical_dimension);
			std::vector<double> res(b.size());

			res = SeidelMethod::solve(submatrix, b);

			if (in_allowable_area(res)) {
				for (size_t i = 0; i < b.size(); ++i) {
					x[vectors_in_basis[i]] = res[i];
				}

				double potential_min = calculate_objective(x);
				if (min > potential_min) {
					optimal = x;
					min = potential_min;
				}
			}
		}
	} while (next_combination(vectors_in_basis, canonical_dimension));

	/*std::cout << "\nMin: " << min;
	std::cout << "\nVector:\n";
	for (auto elem : optimal) {
		std::cout << elem << ' ';
	}
	std::vector<double> vars = back_to_original_vars(optimal);
	std::cout << "Original:\n";
	for (auto elem : vars) {
		std::cout << elem << ' ';
	}*/

	return back_to_original_vars(optimal);
}