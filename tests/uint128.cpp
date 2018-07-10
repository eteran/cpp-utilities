
#include <iostream>
#include "../uint128.h"

using numeric::uint128_t;
using numeric::uint128;

int main() {

	uint128_t x = uint128_t(0xaabbccdd11223344);
	uint128_t y = uint128_t(0xaabbccdd11223344);
	
	
	std::cout << "-----------------" << std::endl;
	uint128_t z = x * y;	
	std::cout << "-----------------" << std::endl;
	std::cout << std::hex << z << std::endl;
	

}
