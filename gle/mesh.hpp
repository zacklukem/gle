#ifndef GLE_MESH_HPP
#define GLE_MESH_HPP

#include <gle/common.hpp>
#include <gle/vao.hpp>
#include <gle/vbo.hpp>
#include <glm/glm.hpp>
#include <vector>

GLE_NAMESPACE_BEGIN

/// @brief A 3d mesh object
///
class Mesh {
public:
  /// @brief Construct a new Mesh object with given triangles, then generate
  ///        surface normals
  ///
  /// @param vertices
  /// @param triangles
  inline Mesh(std::vector<glm::vec3> vertices,
              std::vector<glm::uvec3> triangles);

  /// @brief Calculate surface normals
  ///
  inline void calculate_normals();

  /// @brief Initialize the OpenGL vertex buffers and copy the data to them
  ///
  /// Must be called after GL is initialized
  inline void init_buffers();

  /// @brief Bind the mesh buffers and VAO
  ///
  inline void bind_buffers();

  /// @brief Called after drawing the mesh elements to clean up vertexes
  ///
  /// ## Example:
  ///     mesh->bind_buffers();
  ///     glDrawElements(GL_TRIANGLES, mesh->num_elements(),
  ///                    GL_UNSIGNED_INT, (void *)0);
  ///     mesh->post_draw();
  inline void post_draw();

  /// @brief Get the number of elements (number of triangles times 3)
  ///
  /// @return The number of elements
  inline GLsizei num_elements();

private:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> uvs;
  std::vector<glm::uvec3> triangles;
  VBO<glm::vec3> vertices_vbo;
  VBO<glm::vec3> normals_vbo;
  VBO<glm::vec3> uvs_vbo;
  VBO<glm::uvec3> triangles_vbo;
  VAO vao;
};

GLE_NAMESPACE_END

#include "mesh.inl"

#endif // GLE_MESH_HPP
