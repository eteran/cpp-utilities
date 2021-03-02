cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-hash
	src/MD5.cpp
	src/SHA1.cpp
)

add_library(cpp-utilities::hash
	ALIAS cpp-utilities-hash
)

target_include_directories(cpp-utilities-hash
PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(cpp-utilities-hash
PRIVATE
	cpp-utilities::defaults
)

add_subdirectory(test)
