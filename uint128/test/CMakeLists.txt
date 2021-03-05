cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-uint128-test
	uint128.cpp
)

find_package(Boost REQUIRED)


target_link_libraries(cpp-utilities-uint128-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::uint128
	Boost::boost
)

add_test(
	NAME cpp-utilities-uint128-test
	COMMAND $<TARGET_FILE:cpp-utilities-uint128-test>
)

