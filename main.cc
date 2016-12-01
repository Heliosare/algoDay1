#include "parser.h"
#include <iostream>

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Please give a .csv file to launch this program." << std::endl;
		return (-1);
	}
	Parser parser;
	parser.parse(argv[1]);
	return (0);
}
