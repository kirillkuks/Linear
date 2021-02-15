#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Linear.h"

int main(int argc, char* argv[]) {
	if (argc > 1) {
		Parser parser(argv[1]);
		Linear linear = parser.parse();
	}
	return 0;
}