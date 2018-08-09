# eteran / cpp-utilities / fixed
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-fixed" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::fixed" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")

