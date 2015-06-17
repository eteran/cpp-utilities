# cpp-utilities
Miscellaneous C++11 utility classes and functions

### Arena Allocator

Found in [arena.hpp](https://github.com/eteran/cpp-utilities/blob/master/arena.hpp). This is an implementation of a very efficient fixed block size arena allocator. It allows allocating and freeing back to the arena (if you want to, it isn't necessary), and will use one of two strategies depending on the size of blocks you need. If the blocks are smaller than the size of a pointer, and the arena is relatively small, then it will use a bitmap along with compiler intrinsics to find free blocks. If the the blocks are at least as large as a pointer, it will use a freelist implementation. Template deduction will choose the best backend for you.

Usage is **very** simple:

    // create an arena of 1024 64-bit blocks 
    auto arena = arena::make_arena<uint64_t, 1024>();
    auto p1 = arena.allocate(); // get a single uint64_t sized block
    arena.release(p1);          // free that block back into the system 
                                // (not necessary, the arena will clean itself up)
    
On my system , the allocate function when using the freelist strategy, allocate was **as few a 6 instructions**. Some of which were simple `nullptr` checks.

### Bitset Utility Functions

Found in [bitset.hpp](https://github.com/eteran/cpp-utilities/blob/master/bitset.hpp). This header provides a nice utility function to find the first set bit in a bitset. When possible using GCC intrinsics to do it in O(1) time, but falling back on an iterative implementation when this is not possible.

    std::bitset<32> bs;
    bs[4]  = true;
    bs[10] = true;
    int bit_l = bitset::find_first(bs); // returns 4
	int bit_h = bitset::find_last(bs); // returns 10
    
The function is defined to return `bitset.size()` when no bits are set, this is similar to containers returning `.end()` for find operations.

### Bitwise Operations

[bitwise.hpp](https://github.com/eteran/cpp-utilities/blob/master/bitwise.hpp) provides efficient and type safe rotation operations that will work with any integral type. A future version may be implemented using intrinsics, but for now it's a fairly straight forward shift and mask solution.

    int x = 5;
    int y = bitwise::rotate_right(x, 15);
    int x = bitwise::rotate_left(x, 20);

### String Utility Functions

[string.hpp](https://github.com/eteran/cpp-utilities/blob/master/string.hpp) provides several common string functions such as trimming, upper/lower casing, testing what it starts and ends with, etc.

### Algorithms

[algorithm.hpp](https://github.com/eteran/cpp-utilities/blob/master/algorithm.hpp) is a set of algorithms for general purpose use. Currently there are implementations of variadic min and max functions which are compile time. For example:

	int n = algorithm::static_max(1, 2, 3, 10, 5, 6);
	printf("%d\n", n); // prints 10
	
Because it is a compile time constant, you can also safely use it in templates as well. For example:

	template <int N>
	struct Foo {
		static const int value = N * 2;
	};

	int main() {
		int n = Foo<algorithm::static_max(1, 2, 3, 10, 5, 6)>::value;
		printf("%d\n", n); // prints 20
	}

Of course your compiler will have to have good support for `constexpr` :-).

### Pretty Printers

[pprint.hpp](https://github.com/eteran/cpp-utilities/blob/master/pprint.hpp) is a set of utility functions to print common c++ data structures in a "pretty" way. Similar to PHP's `print_r()`. Usage looks like this:

	std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
	std::cout << pprint::to_string(v) << std::endl;
	
Which will print:

	std::vector<>
	(
	   [0] => 1
	   [1] => 2
	   [2] => 3
	   [3] => 4
	   [4] => 5
	   [5] => 6
	   [6] => 7
	)

`std::vector`, `std::list`, `std::deque`, `std::set`, `std::map` are all supported. Additionally, complex nesting of containers should work just fine. For example, a list of vectors:

	std::list<std::vector<int>> v = { {1, 2, 3}, {4, 5, 6, 7} };
	std::cout << pprint::to_string(v) << std::endl;
	
Will print:

	std::list<>
	(
	   [0] => std::vector<>
	   (
    	   [0] => 1
    	   [1] => 2
    	   [2] => 3
	   )
	   [1] => std::vector<>
	   (
    	   [0] => 4
    	   [1] => 5
    	   [2] => 6
    	   [3] => 7
	   )
	)


### Fixed Point Math
[Fixed.h](https://github.com/eteran/cpp-utilities/blob/master/Fixed.h)

This is a Fixed Point math class for c++. It supports all combinations which add up to a native data types (8.8/16.16/24.8/etc). The template parameters are the number of bits to use as the base type for both the integer and fractional portions, invalid combinations will yield a compiler error, the current implementation makes use of c++-11 static assert to make this more readable. It should be a nice drop in replacement for native float types. Here's an example usage:

	typedef numeric::Fixed<16, 16> fixed;
	fixed f;
	
This will declare a 16.16 fixed point number. Operators are provided though the use of boost::operators. multiplication and division are implemented in free functions named `numeric::multiply` and `numeric::divide` which use `std::enable_if` to choose the best option. If a larger type is available, it will use the accurate and fast scaled math version. If here is not a larger type, then it will fall back on the slower multiply and emulated divide (which unfortunately has less precision). This system allows the user to specialize the multiplication and division as needed.	
