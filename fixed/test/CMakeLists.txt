cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-fixed-test
	fixed.cpp
)
target_link_libraries(cpp-utilities-fixed-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::fixed
)

add_test(
	NAME cpp-utilities-fixed-test
	COMMAND $<TARGET_FILE:cpp-utilities-fixed-test>
)

