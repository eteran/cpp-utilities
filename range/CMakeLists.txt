cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-range INTERFACE)
add_library(cpp-utilities::range ALIAS cpp-utilities-range)

target_include_directories(cpp-utilities-range
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

