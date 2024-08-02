#include "tests/Tests.hpp"

int main(int argc, char*argv[]) {
	int args = 0;

	switch (argc) {
		case 2:
			if (argv[1][0] == 's') {
				args = SILENT;
			} else {
				std::cout << "allowed args are: s" << std::endl;
				return(1);
			}
			break;
		default:
			break;
	}
	Tests testing(args);
	std::cout << "Running tests: " << std::endl;
	testing.testing();
}
