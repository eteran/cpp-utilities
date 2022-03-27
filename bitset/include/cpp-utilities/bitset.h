/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Evan Teran
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef UTILITY_BITSET_HPP_
#define UTILITY_BITSET_HPP_

#include <bitset>

namespace bitset {

template <size_t N>
int find_first(const std::bitset<N> &bs) noexcept {
	if (bs.none()) {
		return N;
	}

	std::size_t i;
	for (i = 0; i < N; ++i) {
		if (bs[i]) {
			break;
		}
	}

	return i;
}

template <size_t N>
int find_last(const std::bitset<N> &bs) noexcept {
	if (bs.none()) {
		return N;
	}

	size_t i = N;
	while (--i) {
		if (bs[i]) {
			break;
		}
	}

	return i;
}

#if defined(__GNUC__)
template <>
inline int find_first(const std::bitset<32> &bs) noexcept {
	if (bs.none()) {
		return bs.size();
	}
	return __builtin_ctz(static_cast<uint32_t>(bs.to_ulong()));
}

template <>
inline int find_last(const std::bitset<32> &bs) noexcept {
	if (bs.none()) {
		return bs.size();
	}
	return bs.size() - __builtin_clz(static_cast<uint32_t>(bs.to_ulong())) - 1;
}

#if __cplusplus >= 201103L
template <>
inline int find_first(const std::bitset<64> &bs) noexcept {
	if (bs.none()) {
		return bs.size();
	}
	return __builtin_ctzll(static_cast<uint64_t>(bs.to_ullong()));
}

template <>
inline int find_last(const std::bitset<64> &bs) noexcept {
	if (bs.none()) {
		return bs.size();
	}
	return bs.size() - __builtin_clzll(static_cast<uint64_t>(bs.to_ullong())) - 1;
}
#endif
#endif

}

#endif
