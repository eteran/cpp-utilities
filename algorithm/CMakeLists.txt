cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-algorithm INTERFACE)
add_library(cpp-utilities::algorithm ALIAS cpp-utilities-algorithm)

target_include_directories(cpp-utilities-algorithm
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

