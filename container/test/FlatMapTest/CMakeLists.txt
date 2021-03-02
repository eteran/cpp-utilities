cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-container-test-FlatMap
	FlatMap.cpp
)

target_link_libraries(cpp-utilities-container-test-FlatMap
PRIVATE
	cpp-utilities::container
	cpp-utilities::defaults
)

add_test(cpp-utilities-container-test-FlatMap cpp-utilities-container-test-FlatMap)

