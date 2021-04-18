cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-time_code-test
	test.cpp
)

target_link_libraries(cpp-utilities-time_code-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::time_code
)

add_test(
	NAME cpp-utilities-time_code-test
	COMMAND $<TARGET_FILE:cpp-utilities-time_code-test>
)

