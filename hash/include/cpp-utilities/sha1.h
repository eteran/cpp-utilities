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

#ifndef SHA1_20151007_H_
#define SHA1_20151007_H_

#include <algorithm>
#include <array>
#include <climits>
#include <cpp-utilities/bitwise.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace hash {

class sha1 {
public:
	struct state {
		static constexpr int BlockSize = 64;

		uint8_t block_[BlockSize]; // 512-bit message blocks
		uint64_t length_   = 0;    // message length in bits
		std::size_t index_ = 0;    // index into message block array
	};

	class digest {
		friend class sha1;

	public:
		std::string to_string() const {
			static const char hexchars[] = "0123456789abcdef";
			std::string str;
			str.reserve(40);

			auto p = std::back_inserter(str);

			for (int i = 0; i < 5; ++i) {
				*p++ = hexchars[(h_[i] & 0xf0000000) >> 0x1c];
				*p++ = hexchars[(h_[i] & 0x0f000000) >> 0x18];
				*p++ = hexchars[(h_[i] & 0x00f00000) >> 0x14];
				*p++ = hexchars[(h_[i] & 0x000f0000) >> 0x10];
				*p++ = hexchars[(h_[i] & 0x0000f000) >> 0x0c];
				*p++ = hexchars[(h_[i] & 0x00000f00) >> 0x08];
				*p++ = hexchars[(h_[i] & 0x000000f0) >> 0x04];
				*p++ = hexchars[(h_[i] & 0x0000000f) >> 0x00];
			}

			return str;
		}

		std::array<uint8_t, 20> bytes() const {
			std::array<uint8_t, 20> b;

			for (int i = 0; i < 5; ++i) {
				b[3 + (i * 4)] = (h_[i] & 0x000000ff);
				b[2 + (i * 4)] = (h_[i] & 0x0000ff00) >> 8;
				b[1 + (i * 4)] = (h_[i] & 0x00ff0000) >> 16;
				b[0 + (i * 4)] = (h_[i] & 0xff000000) >> 24;
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
		uint32_t h_[5] = {
			0x67452301,
			0xefcdab89,
			0x98badcfe,
			0x10325476,
			0xc3d2e1f0};
	};

public:
	template <class In>
	sha1(In first, In last) {
		update(first, last);
	}

	sha1(const std::string &s) {
		update(s);
	}

	sha1()                           = default;
	sha1(const sha1 &other)          = default;
	sha1 &operator=(const sha1 &rhs) = default;

public:
	template <class In>
	sha1 &update(In first, In last) {
		while (first != last) {
			update(*first++);
		}
		return *this;
	}

	sha1 &update(uint8_t byte) {
		state_.block_[state_.index_++] = byte;

		state_.length_ += 8;

		if (state_.index_ == state::BlockSize) {
			process_block(&state_, &digest_);
		}

		return *this;
	}

	sha1 &update(const std::string &s) {
		update(s.begin(), s.end());
		return *this;
	}

public:
	void swap(sha1 &other) {
		using std::swap;

		swap(digest_, other.digest_);
		swap(state_, other.state_);
	}

	void clear();
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

		// Store the message length as the last 8 octets
		s.block_[56] = (s.length_ >> 56) & 0xff;
		s.block_[57] = (s.length_ >> 48) & 0xff;
		s.block_[58] = (s.length_ >> 40) & 0xff;
		s.block_[59] = (s.length_ >> 32) & 0xff;
		s.block_[60] = (s.length_ >> 24) & 0xff;
		s.block_[61] = (s.length_ >> 16) & 0xff;
		s.block_[62] = (s.length_ >> 8) & 0xff;
		s.block_[63] = (s.length_) & 0xff;
		process_block(&s, &d);

		return d;
	}

private:
	static void process_block(state *state, digest *digest) {
		static constexpr uint32_t K[] = {
			0x5a827999,
			0x6ed9eba1,
			0x8f1bbcdc,
			0xca62c1d6};

		uint32_t W[80]; // Word sequence

		//  Initialize the first 16 words in the array W
		for (int t = 0; t < 16; ++t) {
			W[t] = static_cast<uint32_t>(state->block_[t * 4 + 0]) << 24;
			W[t] |= static_cast<uint32_t>(state->block_[t * 4 + 1]) << 16;
			W[t] |= static_cast<uint32_t>(state->block_[t * 4 + 2]) << 8;
			W[t] |= static_cast<uint32_t>(state->block_[t * 4 + 3]);
		}

#if 0
		for(int t = 16; t < 80; ++t) {
			W[t] = bitwise::rotate_left(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);
		}
#else
		// slight improvement if we attempt to do vectorization:
		// http://software.intel.com/en-us/articles/improving-the-performance-of-the-secure-hash-algorithm-1/
		for (int t = 16; t < 32; ++t) {
			W[t] = bitwise::rotate_left(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		}

		for (int t = 32; t < 80; ++t) {
			W[t] = bitwise::rotate_left(W[t - 6] ^ W[t - 16] ^ W[t - 28] ^ W[t - 32], 2);
		}
#endif

		uint32_t A = digest->h_[0];
		uint32_t B = digest->h_[1];
		uint32_t C = digest->h_[2];
		uint32_t D = digest->h_[3];
		uint32_t E = digest->h_[4];

		for (int t = 0; t < 20; ++t) {
			const uint32_t temp = bitwise::rotate_left(A, 5) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];

			E = D;
			D = C;
			C = bitwise::rotate_left(B, 30);
			B = A;
			A = temp;
		}

		for (int t = 20; t < 40; ++t) {
			const uint32_t temp = bitwise::rotate_left(A, 5) + (B ^ C ^ D) + E + W[t] + K[1];

			E = D;
			D = C;
			C = bitwise::rotate_left(B, 30);
			B = A;
			A = temp;
		}

		for (int t = 40; t < 60; ++t) {
			const uint32_t temp = bitwise::rotate_left(A, 5) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];

			E = D;
			D = C;
			C = bitwise::rotate_left(B, 30);
			B = A;
			A = temp;
		}

		for (int t = 60; t < 80; ++t) {
			const uint32_t temp = bitwise::rotate_left(A, 5) + (B ^ C ^ D) + E + W[t] + K[3];

			E = D;
			D = C;
			C = bitwise::rotate_left(B, 30);
			B = A;
			A = temp;
		}

		digest->h_[0] += A;
		digest->h_[1] += B;
		digest->h_[2] += C;
		digest->h_[3] += D;
		digest->h_[4] += E;

		state->index_ = 0;
	}

private:
	state state_;
	digest digest_;
};

}

#endif
