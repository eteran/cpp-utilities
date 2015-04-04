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
    bs[4] = true;
    bs[10] = true;
    int bit = bitset::find_first(bs); // returns 4
    
The function is defined to return `bitset.size()` when no bits are set, this is similar to containers returning `.end()` for find operations.

### Bitwise Operations

[bitwise.hpp](https://github.com/eteran/cpp-utilities/blob/master/bitwise.hpp) provides efficient and type safe rotation operations that will work with any integral type. A future version may be implemented using intrinsics, but for now it's a fairly straight forward shift and mask solution.

    int x = 5;
    int y = bitwise::rotate_right(x, 15);
    int x = bitwise::rotate_left(x, 20);

### String Utility Functions

[string.hpp](https://github.com/eteran/cpp-utilities/blob/master/string.hpp) provides several common string functions such as trimming, upper/lower casing, testing what it starts and ends with, etc.
