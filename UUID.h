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
#include <boost/regex.hpp>
#include "MD5.h"
#include "SHA1.h"

class UUID {
private:
	UUID() {
	}

public:

	static UUID from_string(std::string uuid) {
	
		assert(is_valid(uuid));
		
		uuid.erase(std::remove(uuid.begin(), uuid.end(), '{'), uuid.end()); 	
		uuid.erase(std::remove(uuid.begin(), uuid.end(), '}'), uuid.end());
		uuid.erase(std::remove(uuid.begin(), uuid.end(), '-'), uuid.end());
		
		assert(uuid.size() == 32);
		
		UUID r;

		for(int i = 0; i < 16; ++i) {
			const char ch[3] = { uuid[i * 2], uuid[i * 2 + 1], '\0' };
			const uint8_t v = strtoul(ch, nullptr, 16);
			r.v_[i] = v;
		}

		return r;
	}


	static UUID v3(const UUID &ns, const std::string &name) {
	
		// To determine the version 3 UUID of a given name, the UUID of the namespace (e.g., 6ba7b810-9dad-11d1-80b4-00c04fd430c8 for a domain) 
		// is transformed to a string of bytes corresponding to its hexadecimal digits, concatenated with the input name, hashed with 
		// MD5 yielding 128 bits. Six bits are replaced by fixed values, four of these bits indicate the version, 0011 for version 3. Finally, 
		// the fixed hash is transformed back into the hexadecimal form with hyphens separating the parts relevant in other UUID versions.
	
		UUID r;
				
		std::vector<uint8_t> bin;
		bin.reserve(16 + name.size());
		
		for(int i = 0; i < 16; ++i) {
			bin.push_back(ns[i]);
		}
		
		for(char ch : name) {
			bin.push_back(ch);
		}
		
		auto digest = hash::MD5(bin.begin(), bin.end()).finalize();
		auto bytes = digest.bytes();
		
		for(int i = 0; i < 16; ++i) {
			r.v_[i] = bytes[i];
		}
		
		// ensure correctness
		r.v_[6] = (r.v_[6] & 0x0f) | 0x30; // make version 3
		r.v_[8] = (r.v_[8] & 0x3f) | 0x80; 
		
		return r;
	}

	static UUID v4() {
		UUID r;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0x00, 0xff);	

		r.v_[0]  = dis(gen);
		r.v_[1]  = dis(gen);
		r.v_[2]  = dis(gen);
		r.v_[3]  = dis(gen);
		r.v_[4]  = dis(gen);
		r.v_[5]  = dis(gen);
		r.v_[6]  = (dis(gen) & 0x0f) | 0x40; // make version 4
		r.v_[7]  = dis(gen);
		r.v_[8]  = (dis(gen) & 0x3f) | 0x80; // make version 4
		r.v_[9]  = dis(gen);	
		r.v_[10] = dis(gen);
		r.v_[11] = dis(gen);
		r.v_[12] = dis(gen);
		r.v_[13] = dis(gen);
		r.v_[14] = dis(gen);
		r.v_[15] = dis(gen);		

		return r;
	}

	static UUID v5(const UUID &ns, const std::string &name) {
	
		// To determine the version 5 UUID of a given name, the UUID of the namespace (e.g., 6ba7b810-9dad-11d1-80b4-00c04fd430c8 for a domain) 
		// is transformed to a string of bytes corresponding to its hexadecimal digits, concatenated with the input name, hashed with 
		// SHA1 yielding 160 bits. Six bits are replaced by fixed values, four of these bits indicate the version, 0011 for version 5. Finally, 
		// the first 128 bits of the fixed hash is transformed back into the hexadecimal form with hyphens separating the parts relevant in other UUID versions.
	
		UUID r;
				
		std::vector<uint8_t> bin;
		bin.reserve(16 + name.size());
		
		for(int i = 0; i < 16; ++i) {
			bin.push_back(ns[i]);
		}
		
		for(char ch : name) {
			bin.push_back(ch);
		}
		
		auto digest = hash::SHA1(bin.begin(), bin.end()).finalize();
		auto bytes = digest.bytes();
		
		for(int i = 0; i < 16; ++i) {
			r.v_[i] = bytes[i];
		}
		
		// ensure correctness
		r.v_[6] = (r.v_[6] & 0x0f) | 0x50; // make version 5
		r.v_[8] = (r.v_[8] & 0x3f) | 0x80; 
		
		return r;
	}

	static bool is_valid(const std::string &uuid) {
		static const std::string uuid_regex_string = "[0-9a-f]{8}-?[0-9a-f]{4}-?[12345][0-9a-f]{3}-?[89ab][0-9a-f]{3}-?[0-9a-f]{12}";
		static const boost::regex uuid_regex("(\\{" + uuid_regex_string + "\\}|" + uuid_regex_string + ")");
		return regex_match(uuid, uuid_regex);
	}
	
	int version() const {
		return (v_[6] >> 4) & 0x0f;
	}
	
	bool operator==(const UUID &rhs) const {
		return std::memcmp(v_, rhs.v_, 16) == 0;
	}
	
	bool operator!=(const UUID &rhs) const {
		return !(*this == rhs);
	}
	
	bool operator<(const UUID &rhs) const {
		return std::memcmp(v_, rhs.v_, 16) < 0;
	}

public:
	std::string to_string() const {
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
	
	uint8_t operator[](size_t index) const {
		assert(index < sizeof(v_));
		return v_[index];	
	}
	
	uint8_t& operator[](size_t index) {
		assert(index < sizeof(v_));
		return v_[index];	
	}	

  
private:
	uint8_t v_[16];
};

#endif
