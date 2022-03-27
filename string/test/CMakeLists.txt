cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-string-test
	test.cpp
)

target_link_libraries(cpp-utilities-string-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::string
)

set_property(TARGET cpp-utilities-string-test PROPERTY CXX_STANDARD 14)


add_test(
	NAME cpp-utilities-string-test
	COMMAND $<TARGET_FILE:cpp-utilities-string-test>
)

