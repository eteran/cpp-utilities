#include "../Fixed.h"
#include <iostream>

using Fixed = numeric::Fixed<16, 16>;

int main() {
	Fixed f = 10.5f;
	std::cout << f << std::endl;
}
