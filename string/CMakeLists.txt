cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-string INTERFACE)
add_library(cpp-utilities::string ALIAS cpp-utilities-string)

target_include_directories(cpp-utilities-string
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(test)

