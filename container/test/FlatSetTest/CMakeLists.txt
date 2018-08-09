# eteran / cpp-utilities / container / test / FlatSetTest
cmake_minimum_required(VERSION 3.5)


project("${PROJECT_NAME}-test-FlatSet" CXX)
add_executable("${PROJECT_NAME}" "")


target_sources(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/FlatSet.cpp"
)


target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		eteran::cpp-utilities::container
)


add_test("${PROJECT_NAME}" "${PROJECT_NAME}")

