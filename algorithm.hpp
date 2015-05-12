
#ifndef UTILITIES_ALGORITHM_HPP
#define UTILITIES_ALGORITHM_HPP

namespace algorithm {

template <size_t A, size_t B, size_t ... Args>
struct static_max {
	static const size_t value = static_max<static_max<A, B>::value, Args...>::value;
};

template <size_t A, size_t B>
struct static_max<A, B> {
	static const size_t value = A > B ? A : B;
};


template <size_t A, size_t B, size_t ... Args>
struct static_min {
	static const size_t value = static_min<static_min<A, B>::value, Args...>::value;
};

template <size_t A, size_t B>
struct static_min<A, B> {
	static const size_t value = A < B ? A : B;
};

}

#endif
