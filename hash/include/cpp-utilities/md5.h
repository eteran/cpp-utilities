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

#ifndef MD5_20070328_H_
#define MD5_20070328_H_

#include <algorithm>
#include <array>
#include <climits>
#include <cpp-utilities/bitwise.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace hash {

namespace detail {

constexpr uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) | (~x & z);
}

constexpr uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
	return (z & x) | (~z & y);
}

constexpr uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
	return x ^ y ^ z;
}

constexpr uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
	return y ^ (x | ~z);
}

template <class Func>
uint32_t xfrm(Func f, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
	a += f(b, c, d) + x + ac;
	a = bitwise::rotate_left(a, s) + b;
	return a;
}

}

class md5 {
public:
	struct state {
		static constexpr int BlockSize = 64;

		uint8_t block_[BlockSize]; // input buffer
		uint64_t length_ = 0;      // number of bits
		size_t index_    = 0;
	};

	class digest {
		friend class md5;

	public:
		std::string to_string() const {
			static const char hexchars[] = "0123456789abcdef";

			std::string str;
			str.reserve(32);

			auto p = std::back_inserter(str);

			for (int i = 0; i < 4; ++i) {
				*p++ = hexchars[(h_[i] & 0x000000f0) >> 0x04];
				*p++ = hexchars[(h_[i] & 0x0000000f) >> 0x00];
				*p++ = hexchars[(h_[i] & 0x0000f000) >> 0x0c];
				*p++ = hexchars[(h_[i] & 0x00000f00) >> 0x08];
				*p++ = hexchars[(h_[i] & 0x00f00000) >> 0x14];
				*p++ = hexchars[(h_[i] & 0x000f0000) >> 0x10];
				*p++ = hexchars[(h_[i] & 0xf0000000) >> 0x1c];
				*p++ = hexchars[(h_[i] & 0x0f000000) >> 0x18];
			}

			return str;
		}

		std::array<uint8_t, 16> bytes() const {
			std::array<uint8_t, 16> b;

			for (int i = 0; i < 4; ++i) {
				b[0 + (i * 4)] = (h_[i] & 0x000000ff);
				b[1 + (i * 4)] = (h_[i] & 0x0000ff00) >> 8;
				b[2 + (i * 4)] = (h_[i] & 0x00ff0000) >> 16;
				b[3 + (i * 4)] = (h_[i] & 0xff000000) >> 24;
			}

			return b;
		}

		bool operator==(const digest &rhs) const {
			return std::memcmp(h_, rhs.h_, sizeof(h_)) == 0;
		}

		bool operator!=(const digest &rhs) const {
			return !(*this == rhs);
		}

	private:
		uint32_t h_[4] = {
			0x67452301,
			0xefcdab89,
			0x98badcfe,
			0x10325476,
		};
	};

public:
	template <class In>
	md5(In first, In last) {
		update(first, last);
	}

	md5(const std::string &s) {
		update(s);
	}

	md5()                          = default;
	md5(const md5 &other)          = default;
	md5 &operator=(const md5 &rhs) = default;

public:
	template <class In>
	md5 &update(In first, In last) {
		while (first != last) {
			update(*first++);
		}
		return *this;
	}

	md5 &update(uint8_t byte) {
		state_.block_[state_.index_++] = byte;

		state_.length_ += 8;

		if (state_.index_ == state::BlockSize) {
			process_block(&state_, &digest_);
		}
		return *this;
	}

	md5 &update(const std::string &s) {
		update(s.begin(), s.end());
		return *this;
	}

public:
	void swap(md5 &other) {
		using std::swap;

		swap(digest_, other.digest_);
		swap(state_, other.state_);
	}

	void clear() {
		digest_ = digest();
		state_  = state();
	}

	digest finalize() const {
		// make copies so this isn't a mutating operation
		state s  = state_;
		digest d = digest_;

		s.block_[s.index_] = 0x80;

		const size_t n = s.index_++;

		if (n > 55) {
			while (s.index_ < state::BlockSize) {
				s.block_[s.index_++] = 0;
			}

			process_block(&s, &d);
		}

		while (s.index_ < 56) {
			s.block_[s.index_++] = 0;
		}

		// append length (before padding)
		s.block_[56] = (s.length_) & 0xff;
		s.block_[57] = (s.length_ >> 8) & 0xff;
		s.block_[58] = (s.length_ >> 16) & 0xff;
		s.block_[59] = (s.length_ >> 24) & 0xff;
		s.block_[60] = (s.length_ >> 32) & 0xff;
		s.block_[61] = (s.length_ >> 40) & 0xff;
		s.block_[62] = (s.length_ >> 48) & 0xff;
		s.block_[63] = (s.length_ >> 56) & 0xff;
		process_block(&s, &d);

		return d;
	}

private:
	static void process_block(state *state, digest *digest) {
		auto FF = [](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
			return detail::xfrm(detail::F, a, b, c, d, x, s, ac);
		};

		auto GG = [](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
			return detail::xfrm(detail::G, a, b, c, d, x, s, ac);
		};

		auto HH = [](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
			return detail::xfrm(detail::H, a, b, c, d, x, s, ac);
		};

		auto II = [](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
			return detail::xfrm(detail::I, a, b, c, d, x, s, ac);
		};

		static constexpr int S11 = 7;
		static constexpr int S12 = 12;
		static constexpr int S13 = 17;
		static constexpr int S14 = 22;
		static constexpr int S21 = 5;
		static constexpr int S22 = 9;
		static constexpr int S23 = 14;
		static constexpr int S24 = 20;
		static constexpr int S31 = 4;
		static constexpr int S32 = 11;
		static constexpr int S33 = 16;
		static constexpr int S34 = 23;
		static constexpr int S41 = 6;
		static constexpr int S42 = 10;
		static constexpr int S43 = 15;
		static constexpr int S44 = 21;

		uint32_t a = digest->h_[0];
		uint32_t b = digest->h_[1];
		uint32_t c = digest->h_[2];
		uint32_t d = digest->h_[3];

		uint32_t x[16];
		std::memcpy(x, state->block_, sizeof(x));

		/* Round 1 */
		a = FF(a, b, c, d, x[0], S11, 0xd76aa478);  /* 1 */
		d = FF(d, a, b, c, x[1], S12, 0xe8c7b756);  /* 2 */
		c = FF(c, d, a, b, x[2], S13, 0x242070db);  /* 3 */
		b = FF(b, c, d, a, x[3], S14, 0xc1bdceee);  /* 4 */
		a = FF(a, b, c, d, x[4], S11, 0xf57c0faf);  /* 5 */
		d = FF(d, a, b, c, x[5], S12, 0x4787c62a);  /* 6 */
		c = FF(c, d, a, b, x[6], S13, 0xa8304613);  /* 7 */
		b = FF(b, c, d, a, x[7], S14, 0xfd469501);  /* 8 */
		a = FF(a, b, c, d, x[8], S11, 0x698098d8);  /* 9 */
		d = FF(d, a, b, c, x[9], S12, 0x8b44f7af);  /* 10 */
		c = FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
		b = FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
		a = FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
		d = FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
		c = FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
		b = FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

		/* Round 2 */
		a = GG(a, b, c, d, x[1], S21, 0xf61e2562);  /* 17 */
		d = GG(d, a, b, c, x[6], S22, 0xc040b340);  /* 18 */
		c = GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
		b = GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);  /* 20 */
		a = GG(a, b, c, d, x[5], S21, 0xd62f105d);  /* 21 */
		d = GG(d, a, b, c, x[10], S22, 0x02441453); /* 22 */
		c = GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
		b = GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);  /* 24 */
		a = GG(a, b, c, d, x[9], S21, 0x21e1cde6);  /* 25 */
		d = GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
		c = GG(c, d, a, b, x[3], S23, 0xf4d50d87);  /* 27 */
		b = GG(b, c, d, a, x[8], S24, 0x455a14ed);  /* 28 */
		a = GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
		d = GG(d, a, b, c, x[2], S22, 0xfcefa3f8);  /* 30 */
		c = GG(c, d, a, b, x[7], S23, 0x676f02d9);  /* 31 */
		b = GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

		/* Round 3 */
		a = HH(a, b, c, d, x[5], S31, 0xfffa3942);  /* 33 */
		d = HH(d, a, b, c, x[8], S32, 0x8771f681);  /* 34 */
		c = HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
		b = HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
		a = HH(a, b, c, d, x[1], S31, 0xa4beea44);  /* 37 */
		d = HH(d, a, b, c, x[4], S32, 0x4bdecfa9);  /* 38 */
		c = HH(c, d, a, b, x[7], S33, 0xf6bb4b60);  /* 39 */
		b = HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
		a = HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
		d = HH(d, a, b, c, x[0], S32, 0xeaa127fa);  /* 42 */
		c = HH(c, d, a, b, x[3], S33, 0xd4ef3085);  /* 43 */
		b = HH(b, c, d, a, x[6], S34, 0x04881d05);  /* 44 */
		a = HH(a, b, c, d, x[9], S31, 0xd9d4d039);  /* 45 */
		d = HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
		c = HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
		b = HH(b, c, d, a, x[2], S34, 0xc4ac5665);  /* 48 */

		/* Round 4 */
		a = II(a, b, c, d, x[0], S41, 0xf4292244);  /* 49 */
		d = II(d, a, b, c, x[7], S42, 0x432aff97);  /* 50 */
		c = II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
		b = II(b, c, d, a, x[5], S44, 0xfc93a039);  /* 52 */
		a = II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
		d = II(d, a, b, c, x[3], S42, 0x8f0ccc92);  /* 54 */
		c = II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
		b = II(b, c, d, a, x[1], S44, 0x85845dd1);  /* 56 */
		a = II(a, b, c, d, x[8], S41, 0x6fa87e4f);  /* 57 */
		d = II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
		c = II(c, d, a, b, x[6], S43, 0xa3014314);  /* 59 */
		b = II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
		a = II(a, b, c, d, x[4], S41, 0xf7537e82);  /* 61 */
		d = II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
		c = II(c, d, a, b, x[2], S43, 0x2ad7d2bb);  /* 63 */
		b = II(b, c, d, a, x[9], S44, 0xeb86d391);  /* 64 */

		digest->h_[0] += a;
		digest->h_[1] += b;
		digest->h_[2] += c;
		digest->h_[3] += d;

		state->index_ = 0;
	}

private:
	state state_;
	digest digest_;
};

}

#endif
