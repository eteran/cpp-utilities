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

#ifndef UTILITIES_STRING_HPP_
#define UTILITIES_STRING_HPP_

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace string {

//------------------------------------------------------------------------------
// Name: safe_ctype
//------------------------------------------------------------------------------
template <int (&F)(int)>
int safe_ctype(unsigned char c) {
	return F(c);
}

//------------------------------------------------------------------------------
// Name: ends_with
//------------------------------------------------------------------------------
inline bool ends_with(const std::string &s, char ch) {
	return !s.empty() && s.back() == ch;
}

//------------------------------------------------------------------------------
// Name: ends_with
//------------------------------------------------------------------------------
inline bool ends_with(const std::string &s, const std::string &suffix) {
	return std::mismatch(suffix.rbegin(), suffix.rend(), s.rbegin()).first == suffix.rend();
}

//------------------------------------------------------------------------------
// Name: starts_with
//------------------------------------------------------------------------------
inline bool starts_with(const std::string &s, char ch) {
	return !s.empty() && s.front() == ch;
}

//------------------------------------------------------------------------------
// Name: starts_with
//------------------------------------------------------------------------------
inline bool starts_with(const std::string &s, const std::string &prefix) {
	return std::mismatch(prefix.begin(), prefix.end(), s.begin()).first == prefix.end();
}

//------------------------------------------------------------------------------
// Name: rtrim
//------------------------------------------------------------------------------
inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !safe_ctype<std::isspace>(ch);
			}).base(),
			s.end());
}

//------------------------------------------------------------------------------
// Name: ltrim
//------------------------------------------------------------------------------
inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !safe_ctype<std::isspace>(ch);
			}));
}

//------------------------------------------------------------------------------
// Name: trim
//------------------------------------------------------------------------------
inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

//------------------------------------------------------------------------------
// Name: rtrim_copy
//------------------------------------------------------------------------------
inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

//------------------------------------------------------------------------------
// Name: ltrim_copy
//------------------------------------------------------------------------------
inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

//------------------------------------------------------------------------------
// Name: trim_copy
//------------------------------------------------------------------------------
inline std::string trim_copy(std::string &s) {
	trim(s);
	return s;
}

//------------------------------------------------------------------------------
// Name: toupper
//------------------------------------------------------------------------------
inline void toupper(std::string &s) {
	std::transform(s.begin(), s.end(), s.begin(), [](int ch) {
		return std::toupper(ch);
	});
}

//------------------------------------------------------------------------------
// Name: tolower
//------------------------------------------------------------------------------
inline void tolower(std::string &s) {
	std::transform(s.begin(), s.end(), s.begin(), [](int ch) {
		return std::tolower(ch);
	});
}

//------------------------------------------------------------------------------
// Name: toupper_copy
//------------------------------------------------------------------------------
inline std::string toupper_copy(std::string s) {
	toupper(s);
	return s;
}

//------------------------------------------------------------------------------
// Name: tolower_copy
//------------------------------------------------------------------------------
inline std::string tolower_copy(std::string s) {
	tolower(s);
	return s;
}

//------------------------------------------------------------------------------
// Name: implode
// Desc: join elements with a char
//------------------------------------------------------------------------------
inline std::string implode(char glue, const std::vector<std::string> &pieces) {
	std::string s;
	if (!pieces.empty()) {
		s.append(pieces[0]);
		for (size_t i = 1; i < pieces.size(); ++i) {
			s.push_back(glue);
			s.append(pieces[i]);
		}
	}
	return s;
}

//------------------------------------------------------------------------------
// Name: implode
// Desc: join elements with a string
//------------------------------------------------------------------------------
inline std::string implode(const std::string &glue, const std::vector<std::string> &pieces) {
	std::string s;
	if (!pieces.empty()) {
		s.append(pieces[0]);
		for (size_t i = 1; i < pieces.size(); ++i) {
			s.append(glue);
			s.append(pieces[i]);
		}
	}
	return s;
}

//------------------------------------------------------------------------------
// Name: explode
//------------------------------------------------------------------------------
inline std::vector<std::string> explode(const std::string &delimeter, const std::string &string, int limit) {
	std::vector<std::string> r;

	if (!string.empty()) {
		if (limit >= 0) {
			if (limit == 0) {
				limit = 1;
			}

			size_t first = 0;
			size_t last  = string.find(delimeter);

			while (last != std::string::npos) {

				if (--limit == 0) {
					break;
				}

				r.emplace_back(string.substr(first, last - first));
				first = last + delimeter.size();
				last  = string.find(delimeter, last + delimeter.size());
			}

			r.emplace_back(string.substr(first));
		} else {
			size_t first = 0;
			size_t last  = string.find(delimeter);

			while (last != std::string::npos) {
				r.emplace_back(string.substr(first, last - first));
				first = last + delimeter.size();
				last  = string.find(delimeter, last + delimeter.size());
			}

			r.emplace_back(string.substr(first));

			while (limit < 0) {
				r.pop_back();
				++limit;
			}
		}
	}

	return r;
}

//------------------------------------------------------------------------------
// Name: explode
//------------------------------------------------------------------------------
inline std::vector<std::string> explode(const std::string &delimeter, const std::string &string) {
	return explode(delimeter, string, std::numeric_limits<int>::max());
}

//------------------------------------------------------------------------------
// Name: explode
//------------------------------------------------------------------------------
inline std::vector<std::string> explode(char delimeter, const std::string &string, int limit) {
	std::vector<std::string> r;

	if (!string.empty()) {
		if (limit >= 0) {
			if (limit == 0) {
				limit = 1;
			}

			size_t first = 0;
			size_t last  = string.find(delimeter);

			while (last != std::string::npos) {

				if (--limit == 0) {
					break;
				}

				r.emplace_back(string.substr(first, last - first));
				first = last + 1;
				last  = string.find(delimeter, last + 1);
			}

			r.emplace_back(string.substr(first));
		} else {
			size_t first = 0;
			size_t last  = string.find(delimeter);

			while (last != std::string::npos) {
				r.emplace_back(string.substr(first, last - first));
				first = last + 1;
				last  = string.find(delimeter, last + 1);
			}

			r.emplace_back(string.substr(first));

			while (limit < 0) {
				r.pop_back();
				++limit;
			}
		}
	}

	return r;
}

//------------------------------------------------------------------------------
// Name: explode
//------------------------------------------------------------------------------
inline std::vector<std::string> explode(char delimeter, const std::string &string) {
	return explode(delimeter, string, std::numeric_limits<int>::max());
}

//------------------------------------------------------------------------------
// Name: split
//------------------------------------------------------------------------------
template <class Op>
void split(const std::string &s, char delim, Op op) {
	std::stringstream ss(s);
	for (std::string item; std::getline(ss, item, delim);) {
		*op++ = item;
	}
}

//------------------------------------------------------------------------------
// Name: split
//------------------------------------------------------------------------------
inline std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

}

#endif
