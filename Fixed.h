// From: https://github.com/eteran/cpp-utilities/edit/master/Fixed.h
// See also: http://stackoverflow.com/questions/79677/whats-the-best-way-to-do-fixed-point-math
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

#ifndef FIXED_H_
#define FIXED_H_

#include <ostream>
#include <exception>
#include <cstddef> // for size_t
#include <cstdint>
#include <type_traits>

#include <boost/operators.hpp>

namespace numeric {

template <size_t I, size_t F>
class Fixed;

namespace detail {

// helper templates to make magic with types :)
// these allow us to determine resonable types from
// a desired size, they also let us infer the next largest type
// from a type which is nice for the division op
template <size_t T>
struct type_from_size {
	static constexpr bool is_specialized = false;
};

#if defined(__GNUC__) && defined(__x86_64__) && !defined(__STRICT_ANSI__)
template <>
struct type_from_size<128> {
	static constexpr bool   is_specialized = true;
	static constexpr size_t size = 128;
	
	using value_type    = __int128;
	using unsigned_type = unsigned __int128;
	using signed_type   = __int128;
	using next_size     = type_from_size<256>;
};
#endif

template <>
struct type_from_size<64> {
	static constexpr bool   is_specialized = true;
	static constexpr size_t size = 64;
	
	using value_type    = int64_t;
	using unsigned_type = std::make_unsigned<value_type>::type;
	using signed_type   = std::make_signed<value_type>::type;
	using next_size     = type_from_size<128>;
};

template <>
struct type_from_size<32> {
	static constexpr bool   is_specialized = true;
	static constexpr size_t size = 32;
	
	using value_type    = int32_t;
	using unsigned_type = std::make_unsigned<value_type>::type;
	using signed_type   = std::make_signed<value_type>::type;
	using next_size     = type_from_size<64>;
};

template <>
struct type_from_size<16> {
	static constexpr bool   is_specialized = true;
	static constexpr size_t size = 16;
	
	using value_type    = int16_t;
	using unsigned_type = std::make_unsigned<value_type>::type;
	using signed_type   = std::make_signed<value_type>::type;
	using next_size     = type_from_size<32>;
};

template <>
struct type_from_size<8> {
	static constexpr bool   is_specialized = true;
	static constexpr size_t size = 8;
	
	using value_type    = int8_t;
	using unsigned_type = std::make_unsigned<value_type>::type;
	using signed_type   = std::make_signed<value_type>::type;
	using next_size     = type_from_size<16>;
};

// this is to assist in adding support for non-native base
// types (for adding big-int support), this should be fine
// unless your bit-int class doesn't nicely support casting
template <class B, class N>
constexpr B next_to_base(const N& rhs) {
	return static_cast<B>(rhs);
}

struct divide_by_zero : std::exception {
};

template <size_t I, size_t F>
Fixed<I,F> divide(const Fixed<I,F> &numerator, const Fixed<I,F> &denominator, Fixed<I,F> &remainder, typename std::enable_if<type_from_size<I+F>::next_size::is_specialized>::type* = nullptr) {

	using next_type = typename Fixed<I,F>::next_type;
	using base_type = typename Fixed<I,F>::base_type;
	constexpr size_t fractional_bits = Fixed<I,F>::fractional_bits;

	next_type t(numerator.to_raw());
	t <<= fractional_bits;

	Fixed<I,F> quotient;

	quotient  = Fixed<I,F>::from_base(next_to_base<base_type>(t / denominator.to_raw()));
	remainder = Fixed<I,F>::from_base(next_to_base<base_type>(t % denominator.to_raw()));

	return quotient;
}

template <size_t I, size_t F>
Fixed<I,F> divide(Fixed<I,F> numerator, Fixed<I,F> denominator, Fixed<I,F> &remainder, typename std::enable_if<!type_from_size<I+F>::next_size::is_specialized>::type* = nullptr) {

	// NOTE(eteran): division is broken for large types :-(
	// especially when dealing with negative quantities

	using base_type     = typename Fixed<I,F>::base_type;
	using unsigned_type = typename Fixed<I,F>::unsigned_type;

	constexpr int bits = Fixed<I,F>::total_bits;

	if(denominator == 0) {
		throw divide_by_zero();
	} else {

		int sign = 0;

		Fixed<I,F> quotient;

		if(numerator < 0) {
			sign ^= 1;
			numerator = -numerator;
		}

		if(denominator < 0) {
			sign ^= 1;
			denominator = -denominator;
		}

		base_type n      = numerator.to_raw();
		base_type d      = denominator.to_raw();
		base_type x      = 1;
		base_type answer = 0;

		// egyptian division algorithm
		while((n >= d) && (((d >> (bits - 1)) & 1) == 0)) {
			x <<= 1;
			d <<= 1;
		}

		while(x != 0) {
			if(n >= d) {
				n      -= d;
				answer += x;
			}

			x >>= 1;
			d >>= 1;
		}

		unsigned_type l1 = n;
		unsigned_type l2 = denominator.to_raw();

		// calculate the lower bits (needs to be unsigned)
		// unfortunately for many fractions this overflows the type still :-/
		const unsigned_type lo = (static_cast<unsigned_type>(n) << F) / denominator.to_raw();

		quotient  = Fixed<I,F>::from_base((answer << F) | lo);
		remainder = n;

		if(sign) {
			quotient = -quotient;
		}

		return quotient;
	}
}

// this is the usual implementation of multiplication
template <size_t I, size_t F>
void multiply(const Fixed<I,F> &lhs, const Fixed<I,F> &rhs, Fixed<I,F> &result, typename std::enable_if<type_from_size<I+F>::next_size::is_specialized>::type* = nullptr) {

	using next_type = typename Fixed<I,F>::next_type;
	using base_type = typename Fixed<I,F>::base_type;

	constexpr size_t fractional_bits = Fixed<I,F>::fractional_bits;

	next_type t (static_cast<next_type>(lhs.to_raw()) * static_cast<next_type>(rhs.to_raw()));
	t >>= fractional_bits;
	
	result = Fixed<I,F>::from_base(next_to_base<base_type>(t));
}

// this is the fall back version we use when we don't have a next size
// it is slightly slower, but is more robust since it doesn't
// require and upgraded type
template <size_t I, size_t F>
void multiply(const Fixed<I,F> &lhs, const Fixed<I,F> &rhs, Fixed<I,F> &result, typename std::enable_if<!type_from_size<I+F>::next_size::is_specialized>::type* = nullptr) {

	using base_type = typename Fixed<I,F>::base_type;

	constexpr size_t fractional_bits = Fixed<I,F>::fractional_bits;
	constexpr size_t integer_mask    = Fixed<I,F>::integer_mask;
	constexpr size_t fractional_mask = Fixed<I,F>::fractional_mask;

	// more costly but doesn't need a larger type
	constexpr base_type a_hi = (lhs.to_raw() & integer_mask) >> fractional_bits;
	constexpr base_type b_hi = (rhs.to_raw() & integer_mask) >> fractional_bits;
	constexpr base_type a_lo = (lhs.to_raw() & fractional_mask);
	constexpr base_type b_lo = (rhs.to_raw() & fractional_mask);

	constexpr base_type x1 = a_hi * b_hi;
	constexpr base_type x2 = a_hi * b_lo;
	constexpr base_type x3 = a_lo * b_hi;
	constexpr base_type x4 = a_lo * b_lo;

	result = Fixed<I,F>::from_base((x1 << fractional_bits) + (x3 + x2) + (x4 >> fractional_bits));
}
}

/*
 * inheriting from boost::operators enables us to be a drop in replacement for base types
 * without having to specify all the different versions of operators manually
 */
template <size_t I, size_t F>
class Fixed : boost::operators<Fixed<I,F>> {
	static_assert(detail::type_from_size<I + F>::is_specialized, "invalid combination of sizes");

public:
	static constexpr size_t fractional_bits = F;
	static constexpr size_t integer_bits    = I;
	static constexpr size_t total_bits      = I + F;

	using base_type_info = detail::type_from_size<total_bits>;

	using base_type     = typename base_type_info::value_type;
	using next_type     = typename base_type_info::next_size::value_type;
	using unsigned_type = typename base_type_info::unsigned_type;

public:
	static constexpr size_t base_size          = base_type_info::size;
	static constexpr base_type fractional_mask = ~(static_cast<unsigned_type>(~base_type(0)) << fractional_bits);	
	static constexpr base_type integer_mask    = ~fractional_mask;

public:
	static constexpr base_type one = base_type(1) << fractional_bits;

public: // constructors
	Fixed()                          = default;
	Fixed(const Fixed &o)            = default;
	Fixed& operator=(const Fixed &o) = default;
	
	template <class Integer>
	constexpr Fixed(Integer n, typename std::enable_if<std::is_integral<Integer>::value>::type* = nullptr) : data_(base_type(n) << fractional_bits) {
	}

	template <class Float>
	constexpr Fixed(Float n, typename std::enable_if<std::is_floating_point<Float>::value>::type* = nullptr) : data_(static_cast<base_type>(n * one)) {
	}

private:
	// this makes it simpler to create a fixed point object from
	// a native type without scaling
	// use "Fixed::from_base" in order to perform this.
	struct NoScale {};

	constexpr Fixed(base_type n, const NoScale &) : data_(n) {
	}

public:
	constexpr static Fixed from_base(base_type n) {
		return Fixed(n, NoScale());
	}

public:	// comparison operators
	constexpr bool operator==(const Fixed &o) const {
		return data_ == o.data_;
	}

	constexpr bool operator<(const Fixed &o) const {
		return data_ < o.data_;
	}

public:	// unary operators
	constexpr bool operator!() const {
		return !data_;
	}

	Fixed operator~() const {
		Fixed t(*this);
		t.data_ = ~t.data_;
		return t;
	}

	Fixed operator-() const {
		Fixed t(*this);
		t.data_ = -t.data_;
		return t;
	}

	constexpr Fixed operator+() const {
		return *this;
	}

	Fixed& operator++() {
		data_ += one;
		return *this;
	}

	Fixed& operator--() {
		data_ -= one;
		return *this;
	}

public:	// basic math operators
	Fixed& operator+=(const Fixed &n) {
		data_ += n.data_;
		return *this;
	}

	Fixed& operator-=(const Fixed &n) {
		data_ -= n.data_;
		return *this;
	}

	Fixed& operator&=(const Fixed &n) {
		data_ &= n.data_;
		return *this;
	}

	Fixed& operator|=(const Fixed &n) {
		data_ |= n.data_;
		return *this;
	}

	Fixed& operator^=(const Fixed &n) {
		data_ ^= n.data_;
		return *this;
	}

	Fixed& operator*=(const Fixed &n) {
		detail::multiply(*this, n, *this);
		return *this;
	}

	Fixed& operator/=(const Fixed &n) {
		Fixed temp;
		*this = detail::divide(*this, n, temp);
		return *this;
	}

	Fixed& operator>>=(const Fixed &n) {
		data_ >>= n.to_int();
		return *this;
	}

	Fixed& operator<<=(const Fixed &n) {
		data_ <<= n.to_int();
		return *this;
	}

public: // conversion to basic types
	constexpr int to_int() const {
		return (data_ & integer_mask) >> fractional_bits;
	}

	constexpr unsigned int to_uint() const {
		return (data_ & integer_mask) >> fractional_bits;
	}

	constexpr float to_float() const {
		return static_cast<float>(data_) / Fixed::one;
	}

	constexpr double to_double() const		{
		return static_cast<double>(data_) / Fixed::one;
	}

	constexpr base_type to_raw() const {
		return data_;
	}

public:
	void swap(Fixed &rhs) {
		using std::swap;
		swap(data_, rhs.data_);
	}

public:
	base_type data_ = 0;
};

// if we have the same fractional portion, but differing integer portions, we trivially upgrade the smaller type
template <size_t I1, size_t I2, size_t F>
typename std::conditional<I1 >= I2, Fixed<I1,F>, Fixed<I2,F>>::type operator+(const Fixed<I1,F> &lhs, const Fixed<I2,F> &rhs) {

	using T = typename std::conditional<
		I1 >= I2,
		Fixed<I1,F>,
		Fixed<I2,F>
	>::type;

	constexpr T l = T::from_base(lhs.to_raw());
	constexpr T r = T::from_base(rhs.to_raw());
	return l + r;
}

template <size_t I1, size_t I2, size_t F>
typename std::conditional<I1 >= I2, Fixed<I1,F>, Fixed<I2,F>>::type operator-(const Fixed<I1,F> &lhs, const Fixed<I2,F> &rhs) {

	using T = typename std::conditional<
		I1 >= I2,
		Fixed<I1,F>,
		Fixed<I2,F>
	>::type;

	constexpr T l = T::from_base(lhs.to_raw());
	constexpr T r = T::from_base(rhs.to_raw());
	return l - r;
}

template <size_t I1, size_t I2, size_t F>
typename std::conditional<I1 >= I2, Fixed<I1,F>, Fixed<I2,F>>::type operator*(const Fixed<I1,F> &lhs, const Fixed<I2,F> &rhs) {

	using T = typename std::conditional<
		I1 >= I2,
		Fixed<I1,F>,
		Fixed<I2,F>
	>::type;

	constexpr T l = T::from_base(lhs.to_raw());
	constexpr T r = T::from_base(rhs.to_raw());
	return l * r;
}

template <size_t I1, size_t I2, size_t F>
typename std::conditional<I1 >= I2, Fixed<I1,F>, Fixed<I2,F>>::type operator/(const Fixed<I1,F> &lhs, const Fixed<I2,F> &rhs) {

	using T = typename std::conditional<
		I1 >= I2,
		Fixed<I1,F>,
		Fixed<I2,F>
	>::type;

	constexpr T l = T::from_base(lhs.to_raw());
	constexpr T r = T::from_base(rhs.to_raw());
	return l / r;
}

template <size_t I, size_t F>
std::ostream &operator<<(std::ostream &os, const Fixed<I,F> &f) {
	os << f.to_double();
	return os;
}

}

#endif
