GLE_NAMESPACE_BEGIN

namespace __internal__ {

inline int vertex_midpoint(std::vector<glm::vec3> &vertices, int a, int b) {
  auto mid = (vertices.at(a) + vertices.at(b)) / 2.0f;
  for (size_t i = 0; i < vertices.size(); i++) {
    if (vertices.at(i) == mid) {
      return i;
    }
  }
  vertices.push_back(mid);
  return vertices.size() - 1;
}

} // namespace __internal__

inline std::shared_ptr<Mesh> make_cube_mesh() {
  auto vertices = std::vector<glm::vec3>();
  auto triangles = std::vector<glm::uvec3>();

  vertices.push_back(glm::vec3(-1.0, -1.0, 1.0));
  vertices.push_back(glm::vec3(1.0, -1.0, 1.0));
  vertices.push_back(glm::vec3(1.0, 1.0, 1.0));
  vertices.push_back(glm::vec3(-1.0, 1.0, 1.0));
  vertices.push_back(glm::vec3(-1.0, -1.0, -1.0));
  vertices.push_back(glm::vec3(1.0, -1.0, -1.0));
  vertices.push_back(glm::vec3(1.0, 1.0, -1.0));
  vertices.push_back(glm::vec3(-1.0, 1.0, -1.0));

  triangles.push_back(glm::uvec3(0, 1, 2));
  triangles.push_back(glm::uvec3(2, 3, 0));
  triangles.push_back(glm::uvec3(1, 5, 6));
  triangles.push_back(glm::uvec3(6, 2, 1));
  triangles.push_back(glm::uvec3(7, 6, 5));
  triangles.push_back(glm::uvec3(5, 4, 7));
  triangles.push_back(glm::uvec3(4, 0, 3));
  triangles.push_back(glm::uvec3(3, 7, 4));
  triangles.push_back(glm::uvec3(4, 5, 1));
  triangles.push_back(glm::uvec3(1, 0, 4));
  triangles.push_back(glm::uvec3(3, 2, 6));
  triangles.push_back(glm::uvec3(6, 7, 3));

  return std::make_shared<gle::Mesh>(vertices, triangles);
}

inline std::shared_ptr<Mesh> make_ico_sphere_mesh(int subdivisions) {
  auto vertices = std::vector<glm::vec3>();
  auto triangles = std::vector<glm::uvec3>();
  auto uvs = std::vector<glm::vec2>();

  auto t = (1.0 + sqrt(5.0)) / 2.0;

  vertices.push_back(glm::vec3(-1, t, 0));
  vertices.push_back(glm::vec3(1, t, 0));
  vertices.push_back(glm::vec3(-1, -t, 0));
  vertices.push_back(glm::vec3(1, -t, 0));
  vertices.push_back(glm::vec3(0, -1, t));
  vertices.push_back(glm::vec3(0, 1, t));
  vertices.push_back(glm::vec3(0, -1, -t));
  vertices.push_back(glm::vec3(0, 1, -t));
  vertices.push_back(glm::vec3(t, 0, -1));
  vertices.push_back(glm::vec3(t, 0, 1));
  vertices.push_back(glm::vec3(-t, 0, -1));
  vertices.push_back(glm::vec3(-t, 0, 1));

  triangles.push_back(glm::uvec3(0, 11, 5));
  triangles.push_back(glm::uvec3(0, 5, 1));
  triangles.push_back(glm::uvec3(0, 1, 7));
  triangles.push_back(glm::uvec3(0, 7, 10));
  triangles.push_back(glm::uvec3(0, 10, 11));
  triangles.push_back(glm::uvec3(1, 5, 9));
  triangles.push_back(glm::uvec3(5, 11, 4));
  triangles.push_back(glm::uvec3(11, 10, 2));
  triangles.push_back(glm::uvec3(10, 7, 6));
  triangles.push_back(glm::uvec3(7, 1, 8));
  triangles.push_back(glm::uvec3(3, 9, 4));
  triangles.push_back(glm::uvec3(3, 4, 2));
  triangles.push_back(glm::uvec3(3, 2, 6));
  triangles.push_back(glm::uvec3(3, 6, 8));
  triangles.push_back(glm::uvec3(3, 8, 9));
  triangles.push_back(glm::uvec3(4, 9, 5));
  triangles.push_back(glm::uvec3(2, 4, 11));
  triangles.push_back(glm::uvec3(6, 2, 10));
  triangles.push_back(glm::uvec3(8, 6, 7));
  triangles.push_back(glm::uvec3(9, 8, 1));

  for (int i = 0; i < subdivisions; i++) {
    std::vector<glm::uvec3> new_triangles;

    int j = 0;

    for (auto &triangle : triangles) {
      //       x
      //    zx/_\xy
      //   z/_\/_\y
      //      yz
      int x = triangle.x;
      int y = triangle.y;
      int z = triangle.z;
      int xy = __internal__::vertex_midpoint(vertices, x, y);
      int yz = __internal__::vertex_midpoint(vertices, y, z);
      int zx = __internal__::vertex_midpoint(vertices, z, x);
      new_triangles.push_back(glm::uvec3(x, xy, zx));
      new_triangles.push_back(glm::uvec3(xy, y, yz));
      new_triangles.push_back(glm::uvec3(zx, yz, z));
      new_triangles.push_back(glm::uvec3(zx, xy, yz));
      j++;
    }

    triangles = new_triangles;
  }

  for (auto &vertex : vertices) {
    vertex = glm::normalize(vertex);
    uvs.push_back(glm::vec2(0.5 + atan2(vertex.x, vertex.z) / (2 * M_PI),
                            0.5 - asin(vertex.y) / M_PI));
  }

  return std::make_shared<gle::Mesh>(vertices, uvs, triangles);
}

inline std::shared_ptr<Mesh> make_plane_mesh(int subdivisions) {
  auto vertices = std::vector<glm::vec3>();
  auto uvs = std::vector<glm::vec2>();
  auto triangles = std::vector<glm::uvec3>();

  for (int i = 0; i <= subdivisions; i++) {
    for (int j = 0; j <= subdivisions; j++) {
      auto uv = glm::vec2((float)i / (float)subdivisions,
                          (float)j / (float)subdivisions);
      vertices.push_back(glm::vec3(uv.x, 0, uv.y));
      uvs.push_back(uv);
    }
  }

  for (int i = 0; i < subdivisions; i++) {
    for (int j = 0; j < subdivisions; j++) {
      int top_left = i + (j * (subdivisions + 1));
      int bottom_left = top_left + subdivisions + 1;
      triangles.push_back(glm::uvec3(top_left, top_left + 1, bottom_left + 1));
      triangles.push_back(glm::uvec3(top_left, bottom_left + 1, bottom_left));
    }
  }

  return std::make_shared<Mesh>(vertices, uvs, triangles);
}

GLE_NAMESPACE_END

#ifdef GLE_TEST_CASES

#  include <glm/gtx/string_cast.hpp>

TEST_CASE("__internal__::vertex_midpoint finds midpoint") {
  auto vertices = std::vector<glm::vec3>();

  vertices.push_back(glm::vec3(1, 1, 1));
  vertices.push_back(glm::vec3(2, 2, 2));

  int i = gle::__internal__::vertex_midpoint(vertices, 0, 1);

  CHECK(i == 2);
  CHECK(vertices.size() == 3);
  CHECK(vertices.at(i) == glm::vec3(1.5, 1.5, 1.5));
}

TEST_CASE("make_plane_mesh 1 subdivision") {

  // 0---1
  // | \ |
  // 2---3

  auto mesh = gle::make_plane_mesh();
  CHECK(mesh->vertices().size() == 4);
  CHECK(mesh->vertices().at(0) == glm::vec3(0, 0, 0));
  CHECK(mesh->vertices().at(1) == glm::vec3(0, 0, 1));
  CHECK(mesh->vertices().at(2) == glm::vec3(1, 0, 0));
  CHECK(mesh->vertices().at(3) == glm::vec3(1, 0, 1));
  INFO("tri 0 is: ", glm::to_string(mesh->triangles().at(0)));
  CHECK(mesh->triangles().at(0) == glm::uvec3(0, 1, 3));
  INFO("tri 1 is: ", glm::to_string(mesh->triangles().at(1)));
  CHECK(mesh->triangles().at(1) == glm::uvec3(0, 3, 2));
  int i = 0;
  for (auto &normal : mesh->normals()) {
    INFO("norm #: ", i);
    INFO("norm is: ", glm::to_string(normal));
    CHECK(normal == glm::vec3(0, -1, 0));
    i++;
  }
}

#endif