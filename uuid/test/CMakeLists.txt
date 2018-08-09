# eteran / cpp-utilities / uuid / test
cmake_minimum_required(VERSION 3.5)


project("${PROJECT_NAME}-test" CXX)
add_executable("${PROJECT_NAME}" "")


target_sources(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/UUID.cpp"
)


target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		eteran::cpp-utilities::uuid
)


add_test("${PROJECT_NAME}" "${PROJECT_NAME}")

