GLE_NAMESPACE_BEGIN

inline Mesh::Mesh(std::vector<glm::vec3> vertices,
                  std::vector<glm::ivec3> triangles)
    : vertices(vertices), triangles(triangles), uvs(vertices.size()),
      normals(vertices.size()) {
  calculate_normals();
}

inline void Mesh::calculate_normals() {
  if (normals.size() != vertices.size()) {
    GLE_LOG(GLE_INFO, "Realloc mesh normals array");
    normals = std::vector<glm::vec3>(vertices.size());
  }

  for (auto &triangle : triangles) {
    // xz X xy
    auto norm = glm::cross(vertices.at(triangle.x) - vertices.at(triangle.z),
                           vertices.at(triangle.x) - vertices.at(triangle.y));
    normals.at(triangle.x) += norm;
    normals.at(triangle.y) += norm;
    normals.at(triangle.y) += norm;
  }

  for (auto &normal : normals) {
    normal = glm::normalize(normal);
  }
}

GLE_NAMESPACE_END