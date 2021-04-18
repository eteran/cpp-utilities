cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-algorithm-test
	algorithm.cpp
)

target_link_libraries(cpp-utilities-algorithm-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::algorithm
)

set_property(TARGET cpp-utilities-algorithm-test PROPERTY CXX_STANDARD 17)

add_test(
	NAME cpp-utilities-algorithm-test
	COMMAND $<TARGET_FILE:cpp-utilities-algorithm-test>
)

