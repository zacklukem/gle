cmake_minimum_required(VERSION 3.0)
project(playgl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(basic PUBLIC glad/include)
add_library(glad "glad/src/glad.c")

set(INCLUDE_DIRECTORIES
	PUBLIC .
	PUBLIC ${GLM_INCLUDE_DIR}
	PUBLIC ${GLFW3_INCLUDE_DIR}
)

add_executable(basic examples/basic.cpp)
target_include_directories(basic ${INCLUDE_DIRECTORIES})
target_link_libraries(basic glfw glad)
