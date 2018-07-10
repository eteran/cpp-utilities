#include "../Fixed.h"
#include <iostream>

using fixed = numeric::Fixed<16, 16>;
fixed f;

int main() {
	f = 10.5f;
	std::cout << f << std::endl;
}
