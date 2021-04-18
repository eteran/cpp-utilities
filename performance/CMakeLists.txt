cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-time_code INTERFACE)
add_library(cpp-utilities::time_code ALIAS cpp-utilities-time_code)

target_include_directories(cpp-utilities-time_code
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(test)
