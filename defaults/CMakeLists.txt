# eteran / cpp-utilities / defaults
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-defaults" CXX)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("eteran::cpp-utilities::defaults" ALIAS "${PROJECT_NAME}")


target_compile_options(
	"${PROJECT_NAME}"
	INTERFACE
		-Werror
)

