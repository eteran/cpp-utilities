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

#ifndef UUID_H_
#define UUID_H_

#include <string>
#include <random>
#include <cassert>
#include <algorithm>
#include <cpp-utilities/md5.h>
#include <cpp-utilities/sha1.h>

class uuid {
public:
	uuid() = default;
	uuid(const uuid &) = default;
	uuid& operator=(const uuid &) = default;

public:

	static uuid from_string(std::string str) noexcept {

		assert(is_valid(str));

		// once we know it's valid, we can just strip out the optional
		// stuff we don't really need
		str.erase(std::remove(str.begin(), str.end(), '{'), str.end());
		str.erase(std::remove(str.begin(), str.end(), '}'), str.end());
		str.erase(std::remove(str.begin(), str.end(), '-'), str.end());

		assert(str.size() == 32);

		uuid r;

		for(size_t i = 0; i < 16; ++i) {
			const char ch[3] = { str[i * 2], str[i * 2 + 1], '\0' };
			const auto v = static_cast<uint8_t>(strtoul(ch, nullptr, 16));
			r.v_[i] = v;
		}

		return r;
	}


	static uuid v3(const uuid &ns, const std::string &name) noexcept {

		// To determine the version 3 uuid of a given name, the uuid of the namespace (e.g., 6ba7b810-9dad-11d1-80b4-00c04fd430c8 for a domain)
		// is transformed to a string of bytes corresponding to its hexadecimal digits, concatenated with the input name, hashed with
		// MD5 yielding 128 bits. Six bits are replaced by fixed values, four of these bits indicate the version, 0011 for version 3. Finally,
		// the fixed hash is transformed back into the hexadecimal form with hyphens separating the parts relevant in other uuid versions.

		uuid r;

		std::vector<uint8_t> bin;
		bin.reserve(16 + name.size());

		for(size_t i = 0; i < 16; ++i) {
			bin.push_back(ns[i]);
		}

		for(char ch : name) {
			bin.push_back(static_cast<uint8_t>(ch));
		}

		auto digest = hash::md5(bin.begin(), bin.end()).finalize();
		auto bytes = digest.bytes();

		for(size_t i = 0; i < 16; ++i) {
			r.v_[i] = bytes[i];
		}

		// ensure correctness
		r.v_[6] = (r.v_[6] & 0x0f) | 0x30; // make version 3
		r.v_[8] = (r.v_[8] & 0x3f) | 0x80; // make variant 1

		return r;
	}

	static uuid v4() noexcept {
		uuid r;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0x00, 0xff);

		r.v_[0]  = static_cast<uint8_t>(dis(gen));
		r.v_[1]  = static_cast<uint8_t>(dis(gen));
		r.v_[2]  = static_cast<uint8_t>(dis(gen));
		r.v_[3]  = static_cast<uint8_t>(dis(gen));
		r.v_[4]  = static_cast<uint8_t>(dis(gen));
		r.v_[5]  = static_cast<uint8_t>(dis(gen));
		r.v_[6]  = static_cast<uint8_t>((dis(gen) & 0x0f) | 0x40); // make version 4
		r.v_[7]  = static_cast<uint8_t>(dis(gen));
		r.v_[8]  = static_cast<uint8_t>((dis(gen) & 0x3f) | 0x80); // make variant 1
		r.v_[9]  = static_cast<uint8_t>(dis(gen));
		r.v_[10] = static_cast<uint8_t>(dis(gen));
		r.v_[11] = static_cast<uint8_t>(dis(gen));
		r.v_[12] = static_cast<uint8_t>(dis(gen));
		r.v_[13] = static_cast<uint8_t>(dis(gen));
		r.v_[14] = static_cast<uint8_t>(dis(gen));
		r.v_[15] = static_cast<uint8_t>(dis(gen));

		return r;
	}

	static uuid v5(const uuid &ns, const std::string &name) noexcept {

		// To determine the version 5 uuid of a given name, the uuid of the namespace (e.g., 6ba7b810-9dad-11d1-80b4-00c04fd430c8 for a domain)
		// is transformed to a string of bytes corresponding to its hexadecimal digits, concatenated with the input name, hashed with
		// sha1 yielding 160 bits. Six bits are replaced by fixed values, four of these bits indicate the version, 0011 for version 5. Finally,
		// the first 128 bits of the fixed hash is transformed back into the hexadecimal form with hyphens separating the parts relevant in other uuid versions.

		uuid r;

		std::vector<uint8_t> bin;
		bin.reserve(16 + name.size());

		for(size_t i = 0; i < 16; ++i) {
			bin.push_back(ns[i]);
		}

		for(char ch : name) {
			bin.push_back(static_cast<uint8_t>(ch));
		}

		auto digest = hash::sha1(bin.begin(), bin.end()).finalize();
		auto bytes = digest.bytes();

		for(size_t i = 0; i < 16; ++i) {
			r.v_[i] = bytes[i];
		}

		// ensure correctness
		r.v_[6] = (r.v_[6] & 0x0f) | 0x50; // make version 5
		r.v_[8] = (r.v_[8] & 0x3f) | 0x80; // make variant 1

		return r;
	}

	static bool is_valid(const std::string &uuid) noexcept {

		bool has_braces  = false;
		bool has_dashes  = false;
		int  digit_index = 0;

		auto it = uuid.begin();

		// first test to see if we are dealing with a brace wrapped string
		// if so, consume the brace and note it
		if(it != uuid.end()) {
			if(*it == '{') {
				has_braces = true;
				++it;
			}
		}

		// consume digits as we go
		while(it != uuid.end()) {

			// at the very least, we expect a hex digit
			if(!isxdigit(*it)) {
				break;
			}

			// digits 12 and 16 have specific valid values
			if(digit_index == 12) {
				const char *p = strchr("12345", *it);
				if(!p) {
					return false;
				}
			} else if(digit_index == 16) {
				const char *p = strchr("89abAB", *it);
				if(!p) {
					return false;
				}
			}

			++digit_index;
			++it;

			// if we are at digit 8, then we test to see if there
			// is a dash, if we see one, we want one at 12,16,20 too
			if(it != uuid.end()) {
				switch(digit_index) {
				case 8:
					if(*it == '-') {
						has_dashes = true;
						++it;
					}
					break;
				case 12:
				case 16:
				case 20:
					if(has_dashes) {
						if(*it == '-') {
							++it;
						} else {
							return false;
						}
					}
					break;
				}

			}
		}

		// handle the closing brace
		if(digit_index == 32 && has_braces) {
			if(it == uuid.end()) {
				return false;
			}

			if(*it++ != '}') {
				return false;
			}
		}

		// did we consume the whole string and get 32 digits?
		return (digit_index == 32) && (it == uuid.end());
	}

	int version() const noexcept {
		return (v_[6] >> 4) & 0x0f;
	}

	bool operator==(const uuid &rhs) const noexcept {
		return std::memcmp(v_, rhs.v_, 16) == 0;
	}

	bool operator!=(const uuid &rhs) const noexcept {
		return !(*this == rhs);
	}

	bool operator<(const uuid &rhs) const noexcept {
		return std::memcmp(v_, rhs.v_, 16) < 0;
	}
	
public:
	bool is_null() const noexcept {
		return *this == uuid();
	}
	
	bool is_valid() const noexcept {
		const int ver = version();
		return ver >= 1 && ver <= 5;
	}

public:
	std::string to_string() const noexcept {
		char buffer[37];
		snprintf(buffer, sizeof(buffer), "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			v_[0],
			v_[1],
			v_[2],
			v_[3],
			v_[4],
			v_[5],
			v_[6],
			v_[7],
			v_[8],
			v_[9],
			v_[10],
			v_[11],
			v_[12],
			v_[13],
			v_[14],
			v_[15]);

		return buffer;
	}

	uint8_t operator[](size_t index) const noexcept {
		assert(index < sizeof(v_));
		return v_[index];
	}

	uint8_t& operator[](size_t index) noexcept {
		assert(index < sizeof(v_));
		return v_[index];
	}

private:
	uint8_t v_[16] = {};
};

#endif
