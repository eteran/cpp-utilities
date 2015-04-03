
#ifndef UTILITIES_STRING_HPP_
#define UTILITIES_STRING_HPP_

#include <algorithm>
#include <string>
#include <cctype>

namespace string {

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
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

//------------------------------------------------------------------------------
// Name: ltrim
//------------------------------------------------------------------------------
inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
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
	std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::toupper));
}

//------------------------------------------------------------------------------
// Name: tolower
//------------------------------------------------------------------------------
inline void tolower(std::string &s) {
	std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::tolower));
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

}

#endif
