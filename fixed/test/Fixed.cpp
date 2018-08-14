#include <eteran/cpp-utilities/Fixed.h>
#include <iostream>
#include <cassert>

using Fixed = numeric::Fixed<16, 16>;

#if __cplusplus >= 201402L
#define STATIC_ASSERT14(expr) static_assert((expr), "")
#else
#define STATIC_ASSERT14(expr) assert((expr))
#endif

int main() {
	Fixed f = 10.5f;
	std::cout << f << std::endl;

	// TODO(eteran): perform these tests on the bit-patterns, as float conversions aren't ideal to use
	constexpr numeric::Fixed<8, 8> a8 = 50.25;
	STATIC_ASSERT14(Fixed(a8) == Fixed(50.25));
	
	// TODO(eteran): perform these tests on the bit-patterns, as float comparisons aren't ideal to use
	STATIC_ASSERT14((Fixed(10.5) * 3)          == 31.5);
	STATIC_ASSERT14((Fixed(10.5) * Fixed(3))   == 31.5);
	STATIC_ASSERT14((3 * Fixed(10.5))          == 31.5);
	STATIC_ASSERT14((Fixed(10.5) * 3.0)        == 31.5);
	STATIC_ASSERT14((Fixed(10.5) * Fixed(3.0)) == 31.5);
	STATIC_ASSERT14((3.0 * Fixed(10.5))        == 31.5);
	
	STATIC_ASSERT14(Fixed(50) / Fixed(5) == Fixed(10));
	
	STATIC_ASSERT14(-Fixed(10) == -10);
	STATIC_ASSERT14(+Fixed(10) == +10);
	
	STATIC_ASSERT14(-Fixed(10) - 5 == -15);
	
	STATIC_ASSERT14(++Fixed(5) == Fixed(6));
	STATIC_ASSERT14(Fixed(5)++ == Fixed(5));
	STATIC_ASSERT14(--Fixed(5) == Fixed(4));
	STATIC_ASSERT14(Fixed(5)-- == Fixed(5));
	
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
