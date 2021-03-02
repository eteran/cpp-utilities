cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-container INTERFACE)
add_library(cpp-utilities::container ALIAS cpp-utilities-container)

target_include_directories(cpp-utilities-container
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(test)


