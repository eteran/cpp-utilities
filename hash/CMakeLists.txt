# eteran / cpp-utilities / hash
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-hash" CXX)
add_library("${PROJECT_NAME}" STATIC "")
add_library("eteran::cpp-utilities::hash" ALIAS "${PROJECT_NAME}")


target_sources(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/src/MD5.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/SHA1.cpp"
)


target_include_directories(
	"${PROJECT_NAME}"
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/include"
)


target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		eteran::cpp-utilities::defaults
)


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")
