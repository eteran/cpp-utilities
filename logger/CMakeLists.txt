# eteran / cpp-utilities / logger
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-logger" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::logger" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)

