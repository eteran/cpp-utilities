
#include "../arena.h"
#include <stdint.h>

int main() {
	auto arena = memory::make_arena<sizeof(uint64_t), 1000>();
	int *x = static_cast<int *>(arena.allocate()); 
	arena.release(x);
}
