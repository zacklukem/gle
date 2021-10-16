add_custom_target(toml_scene_hpp
  DEPENDS glescene toml_scene.toml
)
add_custom_command(
  TARGET toml_scene_hpp
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glescene toml_scene.toml -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/toml_scene.hpp
  BYPRODUCTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/toml_scene.hpp
)

add_executable(basic basic.cpp)
target_include_directories(basic ${GL_INCLUDE_DIRECTORIES})
target_link_libraries(basic ${GL_LIBS})

add_executable(toml_scene toml_scene.cpp)
target_include_directories(toml_scene
  PUBLIC ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  ${GL_INCLUDE_DIRECTORIES}
)
add_dependencies(toml_scene toml_scene_hpp)
target_link_libraries(toml_scene ${GL_LIBS})