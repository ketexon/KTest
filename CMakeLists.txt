cmake_minimum_required(VERSION 3.30)

project(KTest)

add_library(
	KTest
	STATIC
	src/KTest.cpp include/ktest/KTest.hpp
)

add_library(
	KTestWithMain
	STATIC
	src/Main.cpp
)

target_include_directories(
	KTest
	PUBLIC
	include
)

set_target_properties(
	KTest
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_link_libraries(
	KTestWithMain
	PUBLIC
	KTest
)