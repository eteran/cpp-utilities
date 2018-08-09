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
 
#ifndef UTILITIES_PPRINT_HPP_
#define UTILITIES_STRING_HPP_

#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>


namespace pprint {
namespace detail {

template <class T> std::string to_string(const std::vector<T> &c, int indent);
template <class T> std::string to_string(const std::list<T> &c, int indent);
template <class T> std::string to_string(const std::deque<T> &c, int indent);
template <class T> std::string to_string(const std::set<T> &c, int indent);
template <class T1, class T2> std::string to_string(const std::map<T1, T2> &c, int indent);

std::string to_string(const std::string &s, int = 0) {
	return "\"" + s + "\"";
}

std::string to_string(int n, int = 0) {
	return std::to_string(n);
}


template <class C>
std::string to_string_container(const std::string &type, const C &c, int indent) {
	std::stringstream ss;
	typename C::size_type i = 0;
	
	ss << std::setw(indent * 4) << type << std::endl;
	ss << std::setw(indent * 4) << "(" << std::endl;
	++indent;
	for(typename C::const_iterator it = c.begin(); it != c.end(); ++it) {
		ss << std::setw(indent * 4) << '[' << i++ << "] => " << to_string(*it, indent) << std::endl;
	}
	--indent;
	ss << std::setw(indent * 4) << ")";
	return ss.str();
}

template <class C>
std::string to_string_assoc_container(const std::string &type, const C &c, int indent) {
	std::stringstream ss;
	
	ss << std::setw(indent * 4) << type << std::endl;
	ss << std::setw(indent * 4) << "(" << std::endl;
	++indent;
	for(typename C::const_iterator it = c.begin(); it != c.end(); ++it) {
		ss << std::setw(indent * 4) << '[' << to_string(it->first) << "] => " << to_string(it->second, indent) << std::endl;
	}
	--indent;
	ss << std::setw(indent * 4) << ")";
	return ss.str();
}

template <class T>
std::string to_string(const std::vector<T> &c, int indent) {
	return to_string_container("std::vector<>", c, indent);
}

template <class T>
std::string to_string(const std::list<T> &c, int indent) {
	return to_string_container("std::list<>", c, indent);
}

template <class T>
std::string to_string(const std::deque<T> &c, int indent) {
	return to_string_container("std::deque<>", c, indent);
}

template <class T1, class T2>
std::string to_string(const std::map<T1, T2> &c, int indent) {
	return to_string_assoc_container("std::map<>", c, indent);
}

template <class T>
std::string to_string(const std::set<T> &c, int indent) {
	return to_string_container("std::set<>", c, indent);
}

}

template <class T>
std::string to_string(const std::vector<T> &c) {
	return detail::to_string<T>(c, 0);
}

template <class T>
std::string to_string(const std::list<T> &c) {
	return detail::to_string<T>(c, 0);
}

template <class T>
std::string to_string(const std::deque<T> &c) {
	return detail::to_string<T>(c, 0);
}

template <class T>
std::string to_string(const std::set<T> &c) {
	return detail::to_string<T>(c, 0);
}

template <class T1, class T2>
std::string to_string(const std::map<T1, T2> &c) {
	return detail::to_string<T1, T2>(c, 0);
}


}

#endif
