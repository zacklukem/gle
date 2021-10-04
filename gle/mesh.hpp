#ifndef GLE_MESH_HPP
#define GLE_MESH_HPP

#include <gle/common.hpp>
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
              std::vector<glm::ivec3> triangles);

  /// @brief Calculate surface normals
  ///
  inline void calculate_normals();

private:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> uvs;
  std::vector<glm::ivec3> triangles;
};

GLE_NAMESPACE_END

#include "mesh.inl"

#endif // GLE_MESH_HPP
