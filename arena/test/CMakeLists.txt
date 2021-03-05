
cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-arena-test
	Arena.cpp
)

target_link_libraries(cpp-utilities-arena-test
PRIVATE
	cpp-utilities::arena
	cpp-utilities::defaults
)


add_test(
	NAME cpp-utilities-arena-test
	COMMAND $<TARGET_FILE:cpp-utilities-arena-test>
)

