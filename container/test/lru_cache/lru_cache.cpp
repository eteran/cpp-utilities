
#include <cpp-utilities/lru_cache.h>
#include <cstdint>
#include <iostream>
#include <memory>

int main() {

	lru_cache<uint64_t, std::unique_ptr<uint8_t[]>> cache(10);
	uint64_t address = 1024 * 1024 * 50;

	for (int i = 0; i < 20; ++i) {
		cache.insert(address + i, std::make_unique<uint8_t[]>(4096));
	}

	std::cout << "CACHE SIZE: " << cache.size() << std::endl;

	if (auto p = cache.take(address)) {
		std::cout << "Address: " << (void *)p->get() << std::endl;
	}

	if (auto p = cache.take(address + 4)) {
		std::cout << "Address: " << (void *)p->get() << std::endl;
	}

	if (auto p = cache.take(address + 9)) {
		std::cout << "Address: " << (void *)p->get() << std::endl;
	}

	for (auto key : cache.keys()) {
		std::cout << "KEY: " << key << std::endl;
	}

	std::cout << "CACHE SIZE: " << cache.size() << std::endl;
}
