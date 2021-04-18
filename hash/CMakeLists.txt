cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-hash INTERFACE)
add_library(cpp-utilities::hash ALIAS cpp-utilities-hash)

target_include_directories(cpp-utilities-hash
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(cpp-utilities-hash
INTERFACE
	cpp-utilities::bitwise
)

add_subdirectory(test)
