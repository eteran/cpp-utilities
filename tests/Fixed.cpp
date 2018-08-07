#include "../Fixed.h"
#include <iostream>
#include <cassert>

using Fixed = numeric::Fixed<16, 16>;

int main() {
	Fixed f = 10.5f;
	std::cout << f << std::endl;
	
	assert((Fixed(10.5) * 3)          == 31.5);
	assert((Fixed(10.5) * Fixed(3))   == 31.5);
	assert((3 * Fixed(10.5))          == 31.5);
	assert((Fixed(10.5) * 3.0)        == 31.5);
	assert((Fixed(10.5) * Fixed(3.0)) == 31.5);
	assert((3.0 * Fixed(10.5))        == 31.5);
	
	assert((-Fixed(10))        == -10);
	
	
	// test some constexpr comparisons stuff
	static_assert(Fixed{1} >    Fixed{0}, "");
	static_assert(Fixed{0.5} <  Fixed{1}, "");
	static_assert(Fixed{1} ==   Fixed{1}, "");
	static_assert(Fixed{0} !=   Fixed{1}, "");
	static_assert(Fixed{1} >=   Fixed{0}, "");
	static_assert(Fixed{0.5} <= Fixed{1}, "");
	
	static_assert(Fixed{1} >   0, "");
	static_assert(Fixed{0.5} < 1, "");
	static_assert(Fixed{1} ==   1, "");
	static_assert(Fixed{0} !=   1, "");
	static_assert(Fixed{1} >=   0, "");
	static_assert(Fixed{0.5} <= 1, "");
	
	static_assert(1 >    Fixed{0}, "");
	static_assert(0.5 <  Fixed{1}, "");
	static_assert(1 ==   Fixed{1}, "");
	static_assert(0 !=   Fixed{1}, "");
	static_assert(1 >=   Fixed{0}, "");
	static_assert(0.5 <= Fixed{1}, "");
}
