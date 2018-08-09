# eteran / cpp-utilities / container
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-container" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::container" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test/FlatMapTest")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test/FlatSetTest")

