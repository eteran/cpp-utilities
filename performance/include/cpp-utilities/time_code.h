
#ifndef TIME_CODE_H_
#define TIME_CODE_H_

#include <chrono>

template <class R, int Count, class F>
R time_code(F func) {

	auto then = std::chrono::system_clock::now();

	for (int i = 0; i < Count; ++i) {
		func();
	}

	auto now = std::chrono::system_clock::now();
	auto dur = now - then;

	return std::chrono::duration_cast<R>(dur);
}

template <class R, class F>
R time_code_once(F func) {
	return time_code<R, 1>(func);
}

#endif
