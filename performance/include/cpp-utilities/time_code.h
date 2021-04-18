
#ifndef TIME_CODE_H_
#define TIME_CODE_H_

#include <chrono>

template <class R, int Count, class F>
R time_code(F func) {

	// test the timing of our printf
	auto then = std::chrono::system_clock::now();

	for (int i = 0; i < Count; ++i) {
		func();
	}

	auto now = std::chrono::system_clock::now();
	auto dur = now - then;

	return std::chrono::duration_cast<R>(dur);
}

#endif
