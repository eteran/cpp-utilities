cmake_minimum_required(VERSION 3.5)

add_library(cpp-utilities-defaults INTERFACE)
add_library(cpp-utilities::defaults ALIAS cpp-utilities-defaults)

target_compile_options(cpp-utilities-defaults
INTERFACE
	-Werror
)

