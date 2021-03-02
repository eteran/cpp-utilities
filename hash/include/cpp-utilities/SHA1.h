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

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace hash {

class SHA1 {
public:
	struct State {
		static constexpr int BlockSize = 64;

		uint8_t     block_[BlockSize]; // 512-bit message blocks	
		uint64_t    length_ = 0;       // message length in bits
		std::size_t index_  = 0;       // index into message block array
	};

	class Digest {
		friend class SHA1;
	public:
		Digest() {
			h_[0] = 0x67452301;
			h_[1] = 0xefcdab89;
			h_[2] = 0x98badcfe;
			h_[3] = 0x10325476;
			h_[4] = 0xc3d2e1f0;
		}
		
	public:
		std::string to_string() const;
		std::array<uint8_t, 20> bytes() const;
		bool operator==(const Digest &rhs) const;
		bool operator!=(const Digest &rhs) const;

	private:
		uint32_t h_[5];
	};
	
public:
	template <class In>
	SHA1(In first, In last) {
		append(first, last);
	}

	SHA1(const std::string &s);
	SHA1()                           = default;
	SHA1(const SHA1 &other)          = default;
	SHA1 &operator=(const SHA1 &rhs) = default;

public:
	template <class In>
	SHA1 &append(In first, In last) {
		while(first != last) {
			append(*first++);
		}
		return *this;
	}

	SHA1 &append(uint8_t byte);
	SHA1 &append(const std::string &s);
	SHA1 &operator<<(uint8_t byte);

public:
	void swap(SHA1 &other);
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
