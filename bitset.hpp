
#ifndef UTILITY_BITSET_HPP_
#define UTILITY_BITSET_HPP_

#include <bitset>

namespace bitset {

template <size_t N>
int find_first(const std::bitset<N> &bs) {
	if(bs.none()) {
		return N;
	}

	std::size_t i;
	for(i = 0; i < N; ++i) {
		if(bs[i]) {
			break;
		}
	}

	return i;	
}

template <size_t N>
int find_last(const std::bitset<N> &bs) {
	if(bs.none()) {
		return N;
	}
	
	size_t i = N;
	while(--i) {
		if(bs[i]) {
			break;
		}
	}
	
	return i;
}

#if defined(__GNUC__)
template <>
inline int find_first(const std::bitset<32> &bs) {
	if(bs.none()) {
		return bs.size();
	}
	return __builtin_ctz(static_cast<uint32_t>(bs.to_ulong()));	
}

template <>
inline int find_last(const std::bitset<32> &bs) {
	if(bs.none()) {
		return bs.size();
	}
	return bs.size() - __builtin_clz(static_cast<uint32_t>(bs.to_ulong())) - 1;
}

#if __cplusplus >= 201103L
template <>
inline int find_first(const std::bitset<64> &bs) {
	if(bs.none()) {
		return bs.size();
	}
	return __builtin_ctzll(static_cast<uint64_t>(bs.to_ullong()));
}

template <>
inline int find_last(const std::bitset<64> &bs) {
	if(bs.none()) {
		return bs.size();
	}
	return bs.size() - __builtin_clzll(static_cast<uint64_t>(bs.to_ullong())) - 1;
}
#endif
#endif

}

#endif
