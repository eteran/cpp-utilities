
#include "cpp-utilities/time_code.h"
#include <iostream>
#include <cstdio>

int main() {

	constexpr int Count = 10;
	using ms = std::chrono::microseconds;

	auto time1 = time_code<ms, Count>([]() {
		printf("Hello World\n");
	});
	
	std::cerr << "printf(\"Hello World\\n\") x 10, Took: " << time1.count() << " \xC2\xB5s to execute." << std::endl;

}
