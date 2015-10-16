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

namespace range {

template <class T>
class range {
public:
	range(T first, T last) : first_(first), last_(last) {
	}
	
public:
	class const_iterator {
	public:
		explicit const_iterator(T value) : value_(value) {
		}
		
		bool operator!=(const const_iterator &rhs) const {
			return value_ != rhs.value_;
		}
		
		bool operator==(const const_iterator &rhs) const {
			return value_ == rhs.value_;
		}

		bool operator<(const const_iterator &rhs) const {
			return value_ < rhs.value_;
		}
		
		T operator*() const {
			return value_;
		}
		
		const_iterator operator++() {
			const_iterator temp(value_);
			++value_; 
			return temp;
		}
		
		const_iterator& operator++(int) {
			++value_; 
			return *this;
		}
		
	private:
		T value_;
	};
	
public:
	const_iterator begin() const  { return const_iterator(first_); }
	const_iterator end() const    { return const_iterator(last_); }
	const_iterator cbegin() const { return const_iterator(first_); }
	const_iterator cend() const   { return const_iterator(last_); }
	
private:
	T first_;
	T last_;
};

template <class T>
range<T> make_range(T first, T last) {
	static_assert(std::is_integral<T>::value, "Only Integral Sequences Are Supported");
	return range<T>(first, last);
}
}

#endif
