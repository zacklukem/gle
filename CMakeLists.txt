cmake_minimum_required(VERSION 3.8)
project(playgl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

add_compile_options(
  -Wall -Wextra -pedantic -Werror -Wno-error=unused-variable
  -Wno-error=unused-parameter -Wno-error=unused-private-field
)

add_library(glad "thirdparty/glad/src/glad.c")
target_include_directories(glad PUBLIC thirdparty/glad/include)

set(INCLUDE_DIRECTORIES
	PUBLIC .
  PUBLIC thirdparty/glad/include
  PUBLIC thirdparty/stb
	PUBLIC ${GLM_INCLUDE_DIRS}
	PUBLIC ${GLFW3_INCLUDE_DIRS}
)

set(LIBS glfw glad)

# Tests
add_executable(tests tests/main.cpp)
target_include_directories(tests ${INCLUDE_DIRECTORIES} PUBLIC thirdparty/doctest/doctest/)
target_link_libraries(tests ${LIBS})
add_custom_target(test DEPENDS tests)
add_custom_command(TARGET test COMMAND "bin/tests")

# Examples
add_executable(basic examples/basic.cpp)
target_include_directories(basic ${INCLUDE_DIRECTORIES})
target_link_libraries(basic ${LIBS})