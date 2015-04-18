
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
namespace {

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
	return to_string<T>(c, 0);
}

template <class T>
std::string to_string(const std::list<T> &c) {
	return to_string<T>(c, 0);
}

template <class T>
std::string to_string(const std::deque<T> &c) {
	return to_string<T>(c, 0);
}

template <class T>
std::string to_string(const std::set<T> &c) {
	return to_string<T>(c, 0);
}

template <class T1, class T2>
std::string to_string(const std::map<T1, T2> &c) {
	return to_string<T1, T2>(c, 0);
}


}

#endif
