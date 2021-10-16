add_executable(glescene scene_compiler.cpp)
target_include_directories(glescene
  PUBLIC ../thirdparty/tomlplusplus/include
  PUBLIC ../thirdparty/doctest/doctest/
)

add_executable(scene_tests
  scene_compiler.cpp
  test.cpp
)
target_include_directories(scene_tests
  PUBLIC ../thirdparty/tomlplusplus/include
  PUBLIC ../thirdparty/doctest/doctest/
)
target_compile_definitions(scene_tests PRIVATE GLE_COMPILE_TESTS)