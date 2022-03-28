cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-container-test-lru_cache
	lru_cache.cpp
)

target_link_libraries(cpp-utilities-container-test-lru_cache
PRIVATE
	cpp-utilities::container
	cpp-utilities::defaults
)

add_test(
	NAME cpp-utilities-container-test-lru_cache
	COMMAND $<TARGET_FILE:cpp-utilities-container-test-lru_cache>
)

set_property(TARGET cpp-utilities-container-test-lru_cache PROPERTY CXX_STANDARD 17)
