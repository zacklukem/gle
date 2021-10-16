cmake_minimum_required(VERSION 3.8)
project(playgl)

include(${PROJECT_SOURCE_DIR}/cmake/globals.cmake)

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
target_include_directories(glad
  PUBLIC ${PROJECT_SOURCE_DIR}/thirdparty/glad/include
)

add_subdirectory(scene)

# Tests
add_executable(tests tests/main.cpp)
target_include_directories(tests
  ${GL_INCLUDE_DIRECTORIES}
  PUBLIC ${PROJECT_SOURCE_DIR}/thirdparty/doctest/doctest/
)
target_link_libraries(tests ${GL_LIBS})
add_custom_target(test DEPENDS tests scene_tests)
add_custom_command(TARGET test
  COMMAND "bin/tests"
  COMMAND "bin/scene_tests"
)

# Examples
add_subdirectory(examples)