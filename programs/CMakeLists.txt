# eteran / cpp-utilities / programs
cmake_minimum_required(VERSION 3.5)


project("eteran-cpp-utilities-programs" CXX)
add_executable("${PROJECT_NAME}" "")


target_sources(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/byte_writer.cpp"
)


target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		eteran::cpp-utilities::defaults
)
