GLE_NAMESPACE_BEGIN

inline Mesh::Mesh(std::vector<glm::vec3> vertices,
                  std::vector<glm::uvec3> triangles)
    : vertices(vertices), triangles(triangles), uvs(vertices.size()),
      normals(vertices.size()), vertices_vbo(GL_ARRAY_BUFFER, false),
      normals_vbo(GL_ARRAY_BUFFER, false), uvs_vbo(GL_ARRAY_BUFFER, false),
      triangles_vbo(GL_ELEMENT_ARRAY_BUFFER, false) {
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

inline void Mesh::init_buffers() {
  vao.init();
  vao.bind();
  vertices_vbo.init();
  normals_vbo.init();
  uvs_vbo.init();
  triangles_vbo.init();

  vertices_vbo.write(vertices);
  normals_vbo.write(normals);
  uvs_vbo.write(uvs);
  triangles_vbo.write(triangles);
}

inline void Mesh::bind_buffers() {
  vao.attr(0, vertices_vbo);
  glEnableVertexAttribArray(0);
  vao.attr(1, normals_vbo);
  glEnableVertexAttribArray(1);
  vao.attr(2, uvs_vbo);
  glEnableVertexAttribArray(2);

  triangles_vbo.bind();
}

inline void Mesh::post_draw() {
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

inline GLsizei Mesh::num_elements() { return triangles.size() * 3; }

GLE_NAMESPACE_END