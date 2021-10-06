GLE_NAMESPACE_BEGIN

inline Mesh::Mesh(std::vector<glm::vec3> vertices,
                  std::vector<glm::uvec3> triangles)
    : _vertices(vertices), _normals(vertices.size()), _uvs(vertices.size()),
      _triangles(triangles), vertices_vbo(GL_ARRAY_BUFFER, false),
      normals_vbo(GL_ARRAY_BUFFER, false), uvs_vbo(GL_ARRAY_BUFFER, false),
      triangles_vbo(GL_ELEMENT_ARRAY_BUFFER, false) {
  calculate_normals();
}

inline Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> uvs,
                  std::vector<glm::uvec3> triangles)
    : _vertices(vertices), _normals(vertices.size()), _uvs(uvs),
      _triangles(triangles), vertices_vbo(GL_ARRAY_BUFFER, false),
      normals_vbo(GL_ARRAY_BUFFER, false), uvs_vbo(GL_ARRAY_BUFFER, false),
      triangles_vbo(GL_ELEMENT_ARRAY_BUFFER, false) {
  calculate_normals();
}

inline void Mesh::calculate_normals() {
  if (_normals.size() != _vertices.size()) {
    GLE_LOG(GLE_INFO, "Realloc mesh normals array");
    _normals = std::vector<glm::vec3>(_vertices.size());
  }

  for (auto &triangle : _triangles) {
    // xz X xy
    auto norm = glm::cross(_vertices.at(triangle.x) - _vertices.at(triangle.z),
                           _vertices.at(triangle.x) - _vertices.at(triangle.y));
    _normals.at(triangle.x) += norm;
    _normals.at(triangle.y) += norm;
    _normals.at(triangle.y) += norm;
  }

  for (auto &normal : _normals) {
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

  vertices_vbo.write(_vertices);
  normals_vbo.write(_normals);
  uvs_vbo.write(_uvs);
  triangles_vbo.write(_triangles);
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

inline GLsizei Mesh::num_elements() { return _triangles.size() * 3; }

inline const std::vector<glm::vec3> &Mesh::vertices() const {
  return _vertices;
}

inline const std::vector<glm::vec3> &Mesh::normals() const { return _normals; }

inline const std::vector<glm::vec3> &Mesh::uvs() const { return _uvs; }

inline const std::vector<glm::uvec3> &Mesh::triangles() const {
  return _triangles;
}

GLE_NAMESPACE_END