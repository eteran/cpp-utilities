cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-uuid-test
	UUID.cpp
)

target_link_libraries(cpp-utilities-uuid-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::uuid
)


add_test(cpp-utilities-uuid-test cpp-utilities-uuid-test)

