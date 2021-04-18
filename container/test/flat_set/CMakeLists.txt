cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-container-test-flat_set
	flat_set.cpp
)

target_link_libraries(cpp-utilities-container-test-flat_set
PRIVATE
	cpp-utilities::container
	cpp-utilities::defaults
)

add_test(
	NAME cpp-utilities-container-test-flat_set
	COMMAND $<TARGET_FILE:cpp-utilities-container-test-flat_set>
)
