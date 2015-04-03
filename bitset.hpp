
#ifndef UTILITY_BITSET_HPP_
#define UTILITY_BITSET_HPP_

#include <bitset>

namespace bitset {
namespace {

// We use a class here because unfortunately, C++ doesn't 
// support partial specialization of template functions

template <std::size_t N>
class bit_finder {
public:	
	int operator()(const std::bitset<N> &bs) const {
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
};

#if defined(__GNUC__)
template <>
class bit_finder<32> {
	static const std::size_t N = 32;
public:
	int operator()(const std::bitset<N> &bs) const {
		if(bs.none()) {
			return N;
		}
		return __builtin_ctz(static_cast<uint32_t>(bs.to_ulong()));	
	}
};

#if __cplusplus >= 201103L
template <>
class bit_finder<64> {
	static const std::size_t N = 64;
public:
	int operator()(const std::bitset<N> &bs) const {
		if(bs.none()) {
			return N;
		}
		return __builtin_ctzll(static_cast<uint64_t>(bs.to_ullong()));
	}
};
#endif
#endif

}

template <size_t N>
int find_first(const std::bitset<N> &bs) {
	auto finder = bit_finder<N>();
	return finder(bs);
}

}

#endif
