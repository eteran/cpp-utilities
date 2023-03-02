#include <cpp-utilities/fixed.h>
#include <iostream>
#include <cassert>

using fixed = numeric::fixed<16, 16>;

#if __cplusplus >= 201402L
#define STATIC_ASSERT14(expr) static_assert((expr), "")
#else
#define STATIC_ASSERT14(expr) assert((expr))
#endif

int main() {
	fixed f = 10.5f;
	std::cout << f << std::endl;

	// TODO(eteran): perform these tests on the bit-patterns, as float conversions aren't ideal to use
	constexpr numeric::fixed<8, 8> a8 = 50.25;
	STATIC_ASSERT14(fixed(a8) == fixed(50.25));

	// TODO(eteran): perform these tests on the bit-patterns, as float comparisons aren't ideal to use
	STATIC_ASSERT14((fixed(10.5) * 3)          == 31.5);
	STATIC_ASSERT14((fixed(10.5) * fixed(3))   == 31.5);
	STATIC_ASSERT14((3 * fixed(10.5))          == 31.5);
	STATIC_ASSERT14((fixed(10.5) * 3.0)        == 31.5);
	STATIC_ASSERT14((fixed(10.5) * fixed(3.0)) == 31.5);
	STATIC_ASSERT14((3.0 * fixed(10.5))        == 31.5);

	STATIC_ASSERT14(fixed(50) / fixed(5) == fixed(10));

	STATIC_ASSERT14(-fixed(10) == -10);
	STATIC_ASSERT14(+fixed(10) == +10);

	STATIC_ASSERT14(-fixed(10) - 5 == -15);

	STATIC_ASSERT14(++fixed(5) == fixed(6));
	STATIC_ASSERT14(fixed(5)++ == fixed(5));
	STATIC_ASSERT14(--fixed(5) == fixed(4));
	STATIC_ASSERT14(fixed(5)-- == fixed(5));

	// test some constexpr comparisons stuff
	static_assert(fixed{1} >    fixed{0}, "");
	static_assert(fixed{0.5} <  fixed{1}, "");
	static_assert(fixed{1} ==   fixed{1}, "");
	static_assert(fixed{0} !=   fixed{1}, "");
	static_assert(fixed{1} >=   fixed{0}, "");
	static_assert(fixed{0.5} <= fixed{1}, "");

	static_assert(fixed{1} >   0, "");
	static_assert(fixed{0.5} < 1, "");
	static_assert(fixed{1} ==   1, "");
	static_assert(fixed{0} !=   1, "");
	static_assert(fixed{1} >=   0, "");
	static_assert(fixed{0.5} <= 1, "");

	static_assert(1 >    fixed{0}, "");
	static_assert(0.5 <  fixed{1}, "");
	static_assert(1 ==   fixed{1}, "");
	static_assert(0 !=   fixed{1}, "");
	static_assert(1 >=   fixed{0}, "");
	static_assert(0.5 <= fixed{1}, "");

	// conversion test
	assert(fixed(0x8000).to_uint() == 0x8000);
}
