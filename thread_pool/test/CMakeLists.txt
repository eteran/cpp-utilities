cmake_minimum_required(VERSION 3.5)

add_executable(cpp-utilities-thread_pool-test
	thread_pool.cpp
)

find_package(Threads REQUIRED)

target_link_libraries(cpp-utilities-thread_pool-test
PRIVATE
	cpp-utilities::defaults
	cpp-utilities::thread_pool
	Threads::Threads
)

add_test(
	NAME cpp-utilities-thread_pool-test
	COMMAND $<TARGET_FILE:cpp-utilities-thread_pool-test>
)

