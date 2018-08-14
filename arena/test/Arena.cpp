#include <eteran/cpp-utilities/arena.h>
#include <stdint.h>

int main() {

	using T = uint64_t;

	auto arena = memory::make_arena<T, 4096>();
	
	auto x1 = static_cast<T *>(arena.allocate()); 
	printf("Allocated Address = %p\n", x1);

	auto x2 = static_cast<T *>(arena.allocate()); 
	printf("Allocated Address = %p\n", x2);
	
	arena.release(x1);
	arena.release(x2);
}
