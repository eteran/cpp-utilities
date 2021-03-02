cmake_minimum_required(VERSION 3.5)

add_executable(byte_writer
	byte_writer.cpp
)

target_link_libraries(byte_writer
PRIVATE
	cpp-utilities::defaults
)
