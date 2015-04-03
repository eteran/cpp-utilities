
#ifndef UTILITIES_BITWISE_HPP_
#define UTILITIES_BITWISE_HPP_

#include <climits>	// for CHAR_BIT

namespace bitwise {

/*
 *  the reason why we need the "mask" constant is 
 *  because some platforms make >> on negative values
 *  arithmetic (which shifts in the sign bit, not 0)
 *  using the mask allows this to work with both signed 
 *  and unsigned types
 */
template <class T>
T rotate_left(T v, int n) {
	static const unsigned int bits = CHAR_BIT * sizeof(T);
	const T mask = ~(T(-1) << n);
	
	return (v << n) | ((v >> (bits - n)) & mask);
}

template <class T>
T rotate_right(T v, int n) {
	return rotate_left(v, -n);
}

}

#endif
