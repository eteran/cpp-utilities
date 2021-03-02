
cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-arena-test
	Arena.cpp
)

target_link_libraries(cpp-utilities-arena-test
PRIVATE
	cpp-utilities::arena
	cpp-utilities::defaults
)


add_test(cpp-utilities-arena-test cpp-utilities-arena-test)

