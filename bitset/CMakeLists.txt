cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-bitset INTERFACE)
add_library(cpp-utilities::bitset ALIAS cpp-utilities-bitset)


target_include_directories(cpp-utilities-bitset
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

