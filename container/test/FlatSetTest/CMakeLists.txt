cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-container-test-FlatSet
	FlatSet.cpp
)

target_link_libraries(cpp-utilities-container-test-FlatSet
PRIVATE
	cpp-utilities::container
	cpp-utilities::defaults
)

add_test(cpp-utilities-container-test-FlatSet cpp-utilities-container-test-FlatSet)

