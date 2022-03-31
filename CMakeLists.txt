cmake_minimum_required(VERSION 3.22)

project(Sudoku LANGUAGES CXX)

get_directory_property(has_parent PARENT_DIRECTORY)
if(has_parent)
	set(has_parent ON)
	set(dev_dflt_enabled OFF)
else()
	set(has_parent OFF)
	set(dev_dflt_enabled ON)
endif()

option(BUILD_TESTING "Build and enable tests." "${dev_dflt_enabled}")

find_package(Catch2 2.11.0 REQUIRED)

file(GLOB_RECURSE all_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.test.cpp")
file(GLOB_RECURSE main_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.main.cpp")

set(lib_sources "${all_sources}")
list(REMOVE_ITEM lib_sources ${test_sources} ${main_sources})

add_library(sudoku-lib ${lib_sources})
target_include_directories(sudoku-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(sudoku-lib PUBLIC cxx_std_20)

# Creates a namespace prefix for the file "target" based on its filepath.
function(prefix_with_namespace INOUT dir)
	set(name "${${INOUT}}")
	if(dir MATCHES "^${CMAKE_CURRENT_SOURCE_DIR}/src/(.*)\$")
		if(CMAKE_MATCH_1)
			string(REPLACE "/" "." prefix "${CMAKE_MATCH_1}")
			set(name "${prefix}.${name}")
		endif()
	endif()
	set("${INOUT}" "${name}" PARENT_SCOPE)
endfunction()

foreach(main ${main_sources})
	get_filename_component(name "${main}" NAME_WE)
	prefix_with_namespace(name "${dir}")
	add_executable("${name}" "${main}")
	target_link_libraries("${name}" PRIVATE sudoku-lib)
endforeach()

if(BUILD_TESTING)
	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/.catch2-main.cpp" "#include <catch2/catch.hpp>")
	add_library(catch2-main "${CMAKE_CURRENT_BINARY_DIR}/.catch2-main.cpp")
	target_compile_definitions(catch2-main PRIVATE CATCH_CONFIG_MAIN)
	target_link_libraries(catch2-main PUBLIC Catch2::Catch2)

	enable_testing()
	foreach(test ${test_sources})
		get_filename_component(name "${test}" NAME_WLE)
		get_filename_component(dir "${test}" DIRECTORY)
		prefix_with_namespace(name "${dir}")
		add_executable("${name}" "${test}")
		# TODO: update it with the actual Catch2 data.
		target_link_libraries("${name}" PRIVATE sudoku-lib catch2-main)

		add_test(NAME "${name}" COMMAND "${name}")
	endforeach()
endif()

