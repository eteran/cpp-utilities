# https://github.com/eteran/cpp-utilities
cmake_minimum_required(VERSION 3.5)


enable_testing()


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/algorithm")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/arena")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/bitset")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/bitwise")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/container")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/defaults")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/fixed")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/hash")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/logger")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/pprint")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/programs")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/range")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/string")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/thread_pool")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/uint128")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/uuid")

