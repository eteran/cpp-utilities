#include <cpp-utilities/algorithm.h>

int main() {
	static_assert(algorithm::static_max(1, 3, 5, 8, 2, 5, 9) == 9);
	static_assert(algorithm::static_min(1, 3, 5, 8, 2, 5, 9) == 1);
}
