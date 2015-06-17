
#ifndef UTILITIES_ALGORITHM_HPP
#define UTILITIES_ALGORITHM_HPP

#include <cstddef>

namespace algorithm {

template <class T>
constexpr T static_max(T n) {
	return n;
}

template <class T, class ... Args>
constexpr T static_max(T n, Args ... args) {
	return n > static_max(args...) ? n : static_max(args...);
}


template <class T>
constexpr T static_min(T n) {
	return n;
}

template <class T, class ... Args>
constexpr T static_min(T n, Args ... args) {
	return n < static_min(args...) ? n : static_min(args...);
}

}

#endif
