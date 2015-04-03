# cpp-utilities
Miscellaneous C++11 utility classes and functions

### Arena Allocator

Found in `arena.hpp`. This is an implementation of a very efficient fixed block size arena allocator. It allows allocating and freeing back to the arena (if you want to, it isn't necessary), and will use one of two strategies depending on the size of blocks you need. If the blocks are smaller than the size of a pointer, and the arena is relatively small, then it will use a bitmap along with compiler intrinsics to find free blocks. If the the blocks are at least as large as a pointer, it will use a freelist implementation.

Usage is **very** simple:

    // create an arena of 1024 64-bit blocks 
    auto arena = arena::make_arena<uint64_t, 1024>();
    auto p1 = arena.allocate(); // get a single uint64_t sized block
    arena.release(p1);          // free that block back into the system (once again not necessary, the arena will clean itself up)
    

### Bitset Utility Functions

### Bitwise Operations

### String Utility Functions
