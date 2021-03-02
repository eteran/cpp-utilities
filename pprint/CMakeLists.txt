cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-pprint INTERFACE)
add_library(cpp-utilities::pprint ALIAS cpp-utilities-pprint)

target_include_directories(cpp-utilities-pprint
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

