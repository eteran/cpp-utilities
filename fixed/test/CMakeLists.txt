cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-fixed-test 
	Fixed.cpp
)
target_link_libraries(cpp-utilities-fixed-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::fixed
)

add_test(cpp-utilities-fixed-test cpp-utilities-fixed-test)

