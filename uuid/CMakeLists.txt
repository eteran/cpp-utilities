# eteran / cpp-utilities / uuid
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-uuid" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::uuid" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)


target_link_libraries(
	"${PROJECT_NAME}"
	INTERFACE
		eteran::cpp-utilities::hash
)


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")

