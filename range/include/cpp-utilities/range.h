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

#ifndef UTILITIES_RANGE_HPP
#define UTILITIES_RANGE_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace range {

namespace detail {

template <class Integer>
class const_iterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type        = Integer;
	using difference_type   = std::ptrdiff_t;
	using pointer           = const Integer; // we lie so std::reverse_iterator<> doesn't return references to temporaries
	using reference         = const Integer; // we lie so std::reverse_iterator<> doesn't return references to temporaries

public:
	explicit const_iterator(Integer value)
		: value_(value) {
	}

	const_iterator(const const_iterator &)            = default;
	const_iterator &operator=(const const_iterator &) = default;

public:
	bool operator!=(const const_iterator &rhs) const { return value_ != rhs.value_; }
	bool operator==(const const_iterator &rhs) const { return value_ == rhs.value_; }
	bool operator<(const const_iterator &rhs) const { return value_ < rhs.value_; }

public:
	Integer operator*() const { return value_; }

public:
	const_iterator &operator++() {
		++value_;
		return *this;
	}

	const_iterator operator++(int) {
		const_iterator tmp(*this);
		++value_;
		return tmp;
	}

	const_iterator &operator--() {
		--value_;
		return *this;
	}

	const_iterator operator--(int) {
		const_iterator tmp(*this);
		--value_;
		return tmp;
	}

private:
	Integer value_;
};

}

template <class Integer>
class numeric_range {
public:
	using size_type              = std::size_t;
	using iterator               = detail::const_iterator<Integer>;
	using const_iterator         = detail::const_iterator<Integer>;
	using reverse_iterator       = std::reverse_iterator<const_iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	numeric_range(Integer first, Integer last)
		: first_(first), last_(last) {
	}

public:
	const_iterator begin() const { return const_iterator(first_); }
	const_iterator cbegin() const { return const_iterator(first_); }
	const_iterator cend() const { return const_iterator(last_); }
	const_iterator end() const { return const_iterator(last_); }
	const_reverse_iterator crbegin() const { return rbegin(); }
	const_reverse_iterator crend() const { return rend(); }
	const_reverse_iterator rbegin() const { return rbegin(); }
	const_reverse_iterator rend() const { return rend(); }
	iterator begin() { return iterator(first_); }
	iterator end() { return iterator(last_); }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }

public:
	size_type size() const { return std::distance(begin(), end()); }
	size_type capacity() const { return size(); }
	bool empty() const { return size() == 0; }

private:
	Integer first_;
	Integer last_;
};

template <class Integer>
numeric_range<Integer> make_numeric_range(Integer first, Integer last) {
	static_assert(std::is_integral<Integer>::value, "Only integral sequences are supported");
	return numeric_range<Integer>(first, last);
}

}

#endif
