
#include <string>
#include <iostream>

char *Lwsreducer(char* input) {
	int i = 0;
	int c = 0;
	while (input[i] != 0) {
		++i;
	}
	char intermediate[i + 1];
	i = 0;
	while (input[i] != 0) {
		intermediate[i] = input[i];
		++i;
	}
	intermediate[i] = 0;
	i = 0;
	while (intermediate[c] != 0) {
		if (intermediate[c] != ' ') {
			input[i] = intermediate[c];
			++c;
		} else {
			input[i] = intermediate[c];
			while (intermediate[c] == ' ') {
				++c;
			}
		}
		++i;
	}
	while (input[i] != 0) {
		input[i] = 0;
		++i;
	}
	return(input);
}

std::string Lwsreducer(std::string& input) {
	std::string output;
	char *test = (char*)input.c_str();
	input = Lwsreducer(test);
	return (input);
}

/*int main(int argc, char* argv[]) {
	std::string test = "     test     test     ";
	int i = 1;

	if (argc <= 1) {
		std::cout << "no inputs detected , falling back on basic test" << std::endl;
		std::cout << test << std::endl;
		std::cout << "to" << std::endl;
		std::cout << Lwsreducer(test) << std::endl;
	} else {
		while (i < argc) {
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << argv[i] << std::endl;
			std::cout << "to" << std::endl;
			std::cout << Lwsreducer(argv[i++]) << std::endl;
		}
	}
	return (0);
}*/