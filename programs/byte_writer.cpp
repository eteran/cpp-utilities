
#include <iostream>
#include <fstream>
#include <cstdio>

namespace {
const int col = 16;
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		std::cerr << "usage: " << argv[0] << " <filename>\n";
		return -1;
	}

	std::ifstream file(argv[1]);

	char ch;
	size_t index = 0;

	while(file.get(ch)) {
		if(index == 0) {
			putchar('"');
		}

		printf("\\x%02x", static_cast<unsigned>(ch) & 0xff);

		index = (index + 1) % col;

		if(index == 0) {
			putchar('"');
			putchar('\n');
		}
	}

	if(index != 0) {
		putchar('"');
		putchar('\n');	
	}

}
