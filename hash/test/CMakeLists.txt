cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-hash-test
	test.cpp
)

target_link_libraries(cpp-utilities-hash-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::hash
)

add_test(cpp-utilities-hash-test cpp-utilities-hash-test)

