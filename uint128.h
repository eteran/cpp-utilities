/*
 * Copyright (c) 2008
 * Evan Teran
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the same name not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission. We make no representations about the
 * suitability this software for any purpose. It is provided "as is"
 * without express or implied warranty.
 */

#ifndef UINT128_20050119_H_
#define UINT128_20050119_H_

#include <stdexcept>
#include <string>
#include <iostream>
#include <climits>
#include <cstdint>

#include <boost/operators.hpp>

namespace numeric {

namespace {
template <typename T>
static void divide(const T &numerator, const T &denominator, T &quotient, T &remainder) {

	static const int bits = sizeof(T) * CHAR_BIT;

	if(denominator == 0) {
		throw std::domain_error("divide by zero");
	} else {
		T n      = numerator;
		T d      = denominator;
		T x      = 1;
		T answer = 0;


		while((n >= d) && (((d >> (bits - 1)) & 1) == 0)) {
			x <<= 1;
			d <<= 1;
		}

		while(x != 0) {
			if(n >= d) {
				n -= d;
				answer |= x;
			}

			x >>= 1;
			d >>= 1;
		}

		quotient = answer;
		remainder = n;
	}
}
}


// convinience macro
#define U128_C(s) uint128(#s)

class uint128 : boost::operators<uint128>, boost::shiftable<uint128> {
public:
	typedef uint64_t base_type;

public:
	static const unsigned int size = (sizeof(base_type) + sizeof(base_type)) * CHAR_BIT;

private:
	base_type lo;
	base_type hi;

public:
	// constructors for all basic types
	uint128()                     : lo(0), hi(0)                             {}
	uint128(int value)            : lo(static_cast<base_type>(value)), hi(0) { if(value < 0) hi = static_cast<base_type>(-1); }
	uint128(unsigned int value)   : lo(static_cast<base_type>(value)), hi(0) {}
	uint128(float value)          : lo(static_cast<base_type>(value)), hi(0) {}
	uint128(double value)         : lo(static_cast<base_type>(value)), hi(0) {}
	uint128(const uint128 &value) : lo(value.lo), hi (value.hi)              {}
	uint128(base_type value)      : lo(value), hi(0)                         {}

	uint128(const std::string &sz) : lo(0), hi(0) {

		// do we have at least one character?
		if(!sz.empty()) {
			// make some reasonable assumptions
			int radix = 10;
			bool minus = false;

			auto i = sz.begin();

			// check for minus sign, i suppose technically this should only apply
			// to base 10, but who says that -0x1 should be invalid?
			if(*i == '-') {
				++i;
				minus = true;
			}

			// check if there is radix changing prefix (0 or 0x)
			if(i != sz.end()) {
				if(*i == '0') {
					radix = 8;
					++i;
					if(i != sz.end()) {
						if(*i == 'x') {
							radix = 16;
							++i;
						}
					}
				}

				while(i != sz.end()) {
					unsigned int n;
					const char ch = *i;

					if(ch >= 'A' && ch <= 'Z') {
						if(((ch - 'A') + 10) < radix) {
							n = (ch - 'A') + 10;
						} else {
							break;
						}
					} else if(ch >= 'a' && ch <= 'z') {
						if(((ch - 'a') + 10) < radix) {
							n = (ch - 'a') + 10;
						} else {
							break;
						}
					} else if(ch >= '0' && ch <= '9') {
						if((ch - '0') < radix) {
							n = (ch - '0');
						} else {
							break;
						}
					} else {
						/* completely invalid character */
						break;
					}

					(*this) *= radix;
					(*this) += n;

					++i;
				}
			}

			// if this was a negative number, do that two's compliment madness :-P
			if(minus) {
				*this = -*this;
			}
		}
	}

	uint128 &operator=(const uint128 &other) {
		if(&other != this) {
			lo = other.lo;
			hi = other.hi;
		}
		return *this;
	}

public: // comparison operators

	bool operator==(const uint128 &o) const {
		return hi == o.hi && lo == o.lo;
	}

	bool operator<(const uint128 &o) const {
		return (hi == o.hi) ? lo < o.lo : hi < o.hi;
	}

public: // unary operators

    bool operator!() const {
		return !(hi != 0 || lo != 0);
	}

    uint128 operator-() const {
		// standard 2's compliment negation
		return ~uint128(*this) + 1;
	}

    uint128 operator~() const {
		uint128 t(*this);
		t.lo = ~t.lo;
		t.hi = ~t.hi;
		return t;
	}

    uint128 &operator++() {
    	if(++lo == 0) {
			++hi;
		}
    	return *this;
	}

    uint128 &operator--() {
    	if(lo-- == 0) {
        	--hi;
		}
    	return *this;
	}

public: // basic math operators

    uint128 &operator+=(const uint128 &b) {
		const base_type old_lo = lo;

    	lo += b.lo;
    	hi += b.hi;

		if(lo < old_lo) {
			++hi;
		}

    	return *this;
	}

    uint128 &operator-=(const uint128 &b) {
		// it happens to be way easier to write it
		// this way instead of make a subtraction algorithm
		return *this += -b;
    }

    uint128 &operator*=(const uint128 &b) {

		// check for multiply by 0
		// result is always 0 :-P
		if(b == 0) {
			hi = 0;
			lo = 0;
		} else if(b != 1) {

			// check we aren't multiplying by 1

    		uint128 a(*this);
    		uint128 t = b;

    		lo = 0;
    		hi = 0;

    		for (unsigned int i = 0; i < size; ++i) {
        		if((t & 1) != 0) {
            		*this += (a << i);
				}

        		t >>= 1;
    		}
		}

    	return *this;
	}

    uint128 &operator|=(const uint128 &b) {
    	hi |= b.hi;
    	lo |= b.lo;
    	return *this;
	}

    uint128 &operator&=(const uint128 &b) {
    	hi &= b.hi;
    	lo &= b.lo;
    	return *this;
	}

    uint128 &operator^=(const uint128 &b) {
    	hi ^= b.hi;
    	lo ^= b.lo;
    	return *this;
	}

    uint128 &operator/=(const uint128 &b) {
		uint128 remainder;
		divide(*this, b, *this, remainder);
		return *this;
    }

    uint128 &operator%=(const uint128 &b) {
		uint128 quotient;
		divide(*this, b, quotient, *this);
		return *this;
    }

    uint128 &operator<<=(const uint128& rhs) {

		unsigned int n = rhs.to_integer();

		if(n >= size) {
			hi = 0;
			lo = 0;
		} else {
			const unsigned int halfsize = size / 2;

    		if(n >= halfsize){
        		n -= halfsize;
        		hi = lo;
        		lo = 0;
    		}

    		if(n != 0) {
				// shift high half
        		hi <<= n;

				const base_type mask(~(base_type(-1) >> n));

				// and add them to high half
        		hi |= (lo & mask) >> (halfsize - n);

				// and finally shift also low half
        		lo <<= n;
    		}
		}

    	return *this;
	}

    uint128 &operator>>=(const uint128& rhs) {

		unsigned int n = rhs.to_integer();

		if(n >= size) {
			hi = 0;
			lo = 0;
		} else {
			const unsigned int halfsize = size / 2;

    		if(n >= halfsize) {
        		n -= halfsize;
        		lo = hi;
        		hi = 0;
    		}

    		if(n != 0) {
				// shift low half
        		lo >>= n;

				// get lower N bits of high half
				const base_type mask(~(base_type(-1) << n));

				// and add them to low qword
        		lo |= (hi & mask) << (halfsize - n);

				// and finally shift also high half
        		hi >>= n;
    		}
		}

    	return *this;
	}

public:
	int to_integer() const {
		return static_cast<int>(lo);
	}

	base_type to_base_type() const {
		return lo;
	}

    std::string to_string(unsigned int radix = 10) const {
    	if(*this == 0) {
			return "0";
		}

    	if(radix < 2 || radix > 37) {
			return "(invalid radix)";
		}

		// at worst it will be size digits (base 2) so make our buffer
		// that plus room for null terminator
    	static char sz [size + 1];
		sz[sizeof(sz) - 1] = '\0';

    	uint128 ii(*this);
    	int i = size - 1;

    	while (ii != 0 && i) {

			uint128 remainder;
			divide(ii, uint128(radix), ii, remainder);
        	sz [--i] = "0123456789abcdefghijklmnopqrstuvwxyz"[remainder.to_integer()];
    	}

    	return &sz[i];
	}
};

std::ostream &operator<<(std::ostream &o, const uint128 &n) {
	switch(o.flags() & (std::ios_base::hex | std::ios_base::dec | std::ios_base::oct)) {
	case std::ios_base::hex:
		o << n.to_string(16);
		break;
	case std::ios_base::dec:
		o << n.to_string(10);
		break;
	case std::ios_base::oct:
		o << n.to_string(8);
		break;
	default:
		o << n.to_string();
		break;
	}
	return o;
}

typedef uint128 uint128_t;

}

#endif
