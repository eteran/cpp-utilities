cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-logger-test
	test.cpp
)

target_link_libraries(cpp-utilities-logger-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::logger
)

add_test(
	NAME cpp-utilities-logger-test
	COMMAND $<TARGET_FILE:cpp-utilities-logger-test>
)

set_property(TARGET cpp-utilities-logger-test PROPERTY CXX_STANDARD 20)

