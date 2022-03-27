
#include "cpp-utilities/string.h"
#include <cassert>

int main() {
	std::string s1 = "\n	hello world \f  ";
	assert(string::rtrim_copy(s1) == "\n	hello world");
	assert(string::ltrim_copy(s1) == "hello world \f  ");
	assert(string::trim_copy(s1) == "hello world");
}
