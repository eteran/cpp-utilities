# eteran / cpp-utilities / uint128 / test
cmake_minimum_required(VERSION 3.5)


project("${PROJECT_NAME}-test" CXX)
add_executable("${PROJECT_NAME}" "")


find_package(Boost REQUIRED)


target_sources(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/uint128.cpp"
)


target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		eteran::cpp-utilities::defaults
		eteran::cpp-utilities::uint128
		Boost::boost
)


add_test("${PROJECT_NAME}" "${PROJECT_NAME}")

