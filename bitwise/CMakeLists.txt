cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-bitwise INTERFACE)
add_library(cpp-utilities::bitwise ALIAS cpp-utilities-bitwise)

target_include_directories(cpp-utilities-bitwise
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

