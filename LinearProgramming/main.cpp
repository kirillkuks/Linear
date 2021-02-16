#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Linear.h"


int main(int argc, char* argv[]) {
	if (argc > 1) {
		Parser parser(argv[1]);
		Linear linear = parser.parse();
		std::vector<double> optimal = linear.solve_task();

		std::cout << "\nOptimal:\n";
		for (auto elem : optimal) {
			std::cout << elem << " ";
		}
	}
	return 0;
}