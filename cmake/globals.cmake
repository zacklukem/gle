set(GL_INCLUDE_DIRECTORIES
	PUBLIC ${CMAKE_SOURCE_DIR}/.
  PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/glad/include
  PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/stb
	PUBLIC ${GLM_INCLUDE_DIRS}
	PUBLIC ${GLFW3_INCLUDE_DIRS}
)

set(GL_LIBS glfw glad)
