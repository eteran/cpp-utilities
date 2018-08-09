# eteran / cpp-utilities / arena
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-arena" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::arena" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/include"
)


target_link_libraries(
	"${PROJECT_NAME}"
	INTERFACE
		eteran::cpp-utilities::bitset
)


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")

