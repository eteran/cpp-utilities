cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-uuid INTERFACE)
add_library(cpp-utilities::uuid ALIAS cpp-utilities-uuid)

target_include_directories(cpp-utilities-uuid
INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(cpp-utilities-uuid
INTERFACE
	cpp-utilities::hash
)

add_subdirectory(test)

