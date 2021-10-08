GLE_NAMESPACE_BEGIN

inline Mesh::Mesh(std::vector<glm::vec3> vertices,
                  std::vector<glm::uvec3> triangles)
    : _vertices(vertices), _normals(vertices.size()),
      _tangents(vertices.size()), _bitangents(vertices.size()),
      _uvs(vertices.size()), _triangles(triangles),
      vertices_vbo(GL_ARRAY_BUFFER, false), normals_vbo(GL_ARRAY_BUFFER, false),
      tangents_vbo(GL_ARRAY_BUFFER, false),
      bitangents_vbo(GL_ARRAY_BUFFER, false), uvs_vbo(GL_ARRAY_BUFFER, false),
      triangles_vbo(GL_ELEMENT_ARRAY_BUFFER, false) {
  calculate_normals();
}

inline Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs,
                  std::vector<glm::uvec3> triangles)
    : _vertices(vertices), _normals(vertices.size()),
      _tangents(vertices.size()), _bitangents(vertices.size()), _uvs(uvs),
      _triangles(triangles), vertices_vbo(GL_ARRAY_BUFFER, false),
      normals_vbo(GL_ARRAY_BUFFER, false), tangents_vbo(GL_ARRAY_BUFFER, false),
      bitangents_vbo(GL_ARRAY_BUFFER, false), uvs_vbo(GL_ARRAY_BUFFER, false),
      triangles_vbo(GL_ELEMENT_ARRAY_BUFFER, false) {
  calculate_normals();
}

inline void Mesh::calculate_normals() {
  if (_normals.size() != _vertices.size()) {
    GLE_LOG(GLE_INFO, "Realloc mesh normals array");
    _normals = std::vector<glm::vec3>(_vertices.size());
  }

  for (auto &normal : _normals) {
    normal = glm::vec3(0);
  }

  for (auto &triangle : _triangles) {
    // xz X xy
    auto norm = glm::cross(_vertices.at(triangle.x) - _vertices.at(triangle.y),
                           _vertices.at(triangle.x) - _vertices.at(triangle.z));
    _normals.at(triangle.x) += norm;
    _normals.at(triangle.y) += norm;
    _normals.at(triangle.z) += norm;
  }

  for (auto &normal : _normals) {
    normal = glm::normalize(normal);
  }

  for (auto &triangle : _triangles) {
    float x1 = _uvs.at(triangle.y).x - _uvs.at(triangle.x).x;
    float x2 = _uvs.at(triangle.z).x - _uvs.at(triangle.x).x;
    float y1 = _uvs.at(triangle.y).y - _uvs.at(triangle.x).y;
    float y2 = _uvs.at(triangle.z).y - _uvs.at(triangle.x).y;
    auto e_1 = _vertices.at(triangle.y) - _vertices.at(triangle.x);
    auto e_2 = _vertices.at(triangle.z) - _vertices.at(triangle.x);
    auto r = 1.0f / (x1 * y2 - y1 * x2);
    auto t = (e_1 * y2 - e_2 * y1) * r;
    auto b = (e_2 * x1 - e_1 * x2) * r;
    _tangents.at(triangle.x) += t;
    _tangents.at(triangle.y) += t;
    _tangents.at(triangle.z) += t;
    _bitangents.at(triangle.x) += b;
    _bitangents.at(triangle.y) += b;
    _bitangents.at(triangle.z) += b;
  }
}

inline void Mesh::init_buffers() {
  vao.init();
  vao.bind();
  vertices_vbo.init();
  normals_vbo.init();
  tangents_vbo.init();
  bitangents_vbo.init();
  uvs_vbo.init();
  triangles_vbo.init();

  vertices_vbo.write(_vertices);
  normals_vbo.write(_normals);
  tangents_vbo.write(_tangents);
  bitangents_vbo.write(_bitangents);
  uvs_vbo.write(_uvs);
  triangles_vbo.write(_triangles);
}

inline void Mesh::bind_buffers() const {
  vao.attr(0, vertices_vbo);
  glEnableVertexAttribArray(0);
  vao.attr(1, normals_vbo);
  glEnableVertexAttribArray(1);
  vao.attr(2, tangents_vbo);
  glEnableVertexAttribArray(2);
  vao.attr(3, bitangents_vbo);
  glEnableVertexAttribArray(3);
  vao.attr(4, uvs_vbo);
  glEnableVertexAttribArray(4);

  triangles_vbo.bind();
}

inline void Mesh::post_draw() const {
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
}

inline GLsizei Mesh::num_elements() const { return _triangles.size() * 3; }

inline const std::vector<glm::vec3> &Mesh::vertices() const {
  return _vertices;
}

inline const std::vector<glm::vec3> &Mesh::normals() const { return _normals; }

inline const std::vector<glm::vec3> &Mesh::tangents() const {
  return _tangents;
}

inline const std::vector<glm::vec3> &Mesh::bitangents() const {
  return _bitangents;
}

inline const std::vector<glm::vec2> &Mesh::uvs() const { return _uvs; }

inline const std::vector<glm::uvec3> &Mesh::triangles() const {
  return _triangles;
}

inline void Mesh::normals(const std::vector<glm::vec3> &normals) {
  _normals = normals;
}

inline void Mesh::uvs(const std::vector<glm::vec2> &uvs) { _uvs = uvs; }

inline void Mesh::draw() const {
  bind_buffers();

  glDrawElements(GL_TRIANGLES, num_elements(), GL_UNSIGNED_INT, (void *)0);

  post_draw();
}

GLE_NAMESPACE_END

#ifdef GLE_TEST_CASES

#  include <gle/meshs/primitives.hpp>

TEST_CASE("normals calculate fast" * doctest::timeout(0.5) *
          doctest::may_fail()) {
  auto mesh = gle::make_ico_sphere_mesh(4);

  INFO("Number of faces: ", mesh->triangles().size());
  for (int i = 0; i < 50; i++) {
    mesh->calculate_normals();
  }
}

#endif