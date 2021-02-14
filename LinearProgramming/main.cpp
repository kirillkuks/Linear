#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Linear.h"

int main(int argc, char* argv[]) {
	if (argc > 1) {
		Matrix m(2, 3);
		m[1][2] = 4;
		std::cout << m[1][2] << "<<<<<<<<<>>>>>>>>>>\n";
		Parser parser(argv[1]);
		parser.parse();
	}
	return 0;
}