cmake_minimum_required(VERSION 3.30)

project(KTest)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(KTEST_STANDALONE ON)
else()
	set(KTEST_STANDALONE OFF)
endif()

if(KTEST_STANDALONE)
	set(KTEST_BUILD_TESTS ON CACHE BOOL "Build tests")
else()
	set(KTEST_BUILD_TESTS OFF CACHE BOOL "Build tests")
endif()


add_library(
	KTest
	STATIC
	include/ktest/KTest.hpp src/KTest.cpp
	src/Assert.cpp
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

target_link_libraries(
	KTestWithMain
	PUBLIC
	KTest
)

set_target_properties(
	KTest KTestWithMain
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

if(KTEST_BUILD_TESTS)
	add_subdirectory(unit)
endif()