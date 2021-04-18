cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-container-test-flat_map
	flat_map.cpp
)

target_link_libraries(cpp-utilities-container-test-flat_map
PRIVATE
	cpp-utilities::container
	cpp-utilities::defaults
)

add_test(
	NAME cpp-utilities-container-test-flat_map
	COMMAND $<TARGET_FILE:cpp-utilities-container-test-flat_map>
)

