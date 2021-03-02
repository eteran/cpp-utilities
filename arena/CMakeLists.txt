
cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-arena INTERFACE)
add_library(cpp-utilities::arena ALIAS cpp-utilities-arena)

target_include_directories(cpp-utilities-arena
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(cpp-utilities-arena
INTERFACE
	cpp-utilities::bitset
)


add_subdirectory(test)

