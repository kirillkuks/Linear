#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Linear.h"


int main(int argc, char* argv[]) {
	if (argc > 1) {
		Parser parser(argv[1]);
		Linear linear = parser.parse();
		std::vector<double> optimal = linear.solve_task();

		std::cout << "\n\nOptimal:\n";
		for (auto elem : optimal) {
			std::cout << elem << " ";
		}

		std::vector<double> dual_optimal = linear.get_dual_program()->solve_task();
		std::cout << "\n\nDual optimal:\n";
		for (auto elem : dual_optimal) {
			std::cout << elem << " ";
		}
	}
	return 0;
}