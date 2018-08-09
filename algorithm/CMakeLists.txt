# eteran / cpp-utilities / algorithm
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-algorithm" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::algorithm" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)

