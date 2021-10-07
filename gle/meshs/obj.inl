#include <fstream>
#include <sstream>

GLE_NAMESPACE_BEGIN

inline std::shared_ptr<Mesh> load_obj_from_file(const std::string &file) {
  std::ifstream stream(file, std::ios_base::in);
  if (!stream) throw std::runtime_error("obj file not found");
  return load_obj(stream);
}

inline std::shared_ptr<Mesh> load_obj(const std::string &source) {
  auto str = std::stringstream(source);
  return load_obj(str);
}

inline std::shared_ptr<Mesh> load_obj(std::istream &source) {
  auto vertices = std::vector<glm::vec3>();
  auto triangles = std::vector<glm::uvec3>();
  auto uvs = std::vector<glm::vec2>();
  auto normals = std::vector<glm::vec3>();

  char c;
  bool isFirst = true;
  while (source.get(c)) {
    if (c == '\n') {
      isFirst = true;
      continue;
    }
    if (!isFirst) continue;

    if (c == 'v') {
      char p = source.peek();
      if (p == 'n') {
        source.get(c);
        glm::vec3 normal;
        source >> normal.x >> normal.y >> normal.z;
        normals.push_back(normal);
      } else if (p == 't') {
        source.get(c);
        glm::vec2 uv;
        source >> uv.x >> uv.y;
        uvs.push_back(uv);
      } else {
        glm::vec3 vertex;
        source >> vertex.x >> vertex.y >> vertex.z;
        vertices.push_back(vertex);
      }
    } else if (c == 'f') {
      glm::uvec3 triangle;
      source >> triangle.x >> triangle.y >> triangle.z;
      triangle -= glm::uvec3(1);
      triangles.push_back(triangle);
    }

    isFirst = false;
  }

  auto mesh = std::make_shared<Mesh>(vertices, triangles);

  if (vertices.size() == uvs.size()) {
    mesh->uvs(uvs);
  }

  if (vertices.size() == normals.size()) {
    mesh->normals(normals);
  }

  return mesh;
}

GLE_NAMESPACE_END

#ifdef GLE_TEST_CASES

TEST_CASE("load_obj parses obj files") {
  std::string obj = R"(
# comment
v 0.1 0.2 0.3 # comment
v 0.4 0.5 0.6
v 0.7 0.8 0.9
vn 1.0 2.0 3.0
vn 4.0 5.0 6.0
vn 7.0 8.0 9.0
vt 1.1 2.2
vt 3.3 4.4
vt 5.5 6.6
f 1 2 3
f 3 2 1
)";
  auto mesh = gle::load_obj(obj);
  CHECK(mesh->vertices().at(0) == glm::vec3(0.1, 0.2, 0.3));
  CHECK(mesh->vertices().at(1) == glm::vec3(0.4, 0.5, 0.6));
  CHECK(mesh->vertices().at(2) == glm::vec3(0.7, 0.8, 0.9));
  CHECK(mesh->normals().at(0) == glm::vec3(1, 2, 3));
  CHECK(mesh->normals().at(1) == glm::vec3(4, 5, 6));
  CHECK(mesh->normals().at(2) == glm::vec3(7, 8, 9));
  CHECK(mesh->uvs().at(0) == glm::vec2(1.1, 2.2));
  CHECK(mesh->uvs().at(1) == glm::vec2(3.3, 4.4));
  CHECK(mesh->uvs().at(2) == glm::vec2(5.5, 6.6));
  CHECK(mesh->triangles().at(0) == glm::uvec3(0, 1, 2));
  CHECK(mesh->triangles().at(1) == glm::uvec3(2, 1, 0));
}

#endif