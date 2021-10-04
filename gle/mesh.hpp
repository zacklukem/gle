#ifndef GLE_MESH_HPP
#define GLE_MESH_HPP

#include <gle/common.hpp>
#include <gle/vao.hpp>
#include <gle/vbo.hpp>
#include <glm/glm.hpp>
#include <vector>

GLE_NAMESPACE_BEGIN

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

  inline void init_buffers();

  inline void bind_buffers();
  inline void post_draw();

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
