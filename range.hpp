
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
