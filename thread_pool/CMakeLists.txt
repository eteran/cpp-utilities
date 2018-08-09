# eteran / cpp-utilities / thread_pool
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-thread_pool" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::thread_pool" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")

