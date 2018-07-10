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

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace hash {

class MD5 {
public:
	struct State {
		static constexpr int BlockSize = 64;

		uint8_t  block_[BlockSize]; // input buffer
		uint64_t length_ = 0;       // number of bits
		size_t   index_  = 0;
	};

	class Digest {
		friend class MD5;
	public:
		Digest() {
			h_[0] = 0x67452301;
			h_[1] = 0xefcdab89;
			h_[2] = 0x98badcfe;
			h_[3] = 0x10325476;
		}
	public:
		std::string to_string() const;
		std::array<uint8_t, 16> bytes() const;
		bool operator==(const Digest &rhs) const;
		bool operator!=(const Digest &rhs) const;

	private:
		uint32_t h_[4];
	};


public:
	template <class In>
	MD5(In first, In last) {
		append(first, last);
	}

	MD5(const std::string &s);
	MD5()                          = default;
	MD5(const MD5 &other)          = default;
	MD5 &operator=(const MD5 &rhs) = default;

public:
	template <class In>
	MD5 &append(In first, In last) {
		while(first != last) {
			append(*first++);
		}
		return *this;
	}

	MD5 &append(uint8_t byte);
	MD5 &append(const std::string &s);
	MD5 &operator<<(uint8_t byte);

public:
	void swap(MD5 &other);
	void clear();
	Digest finalize() const;

private:
	static void processMessageBlock(State *state, Digest *digest);

private:
	State  state_;
	Digest digest_;
};

}

#endif

