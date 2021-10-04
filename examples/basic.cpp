#define GLE_VERBOSE
#include <gle/gle.hpp>

const char *vert = R"(
#version 410
void main() {}
)";

const char *frag = R"(
#version 410
void main() {}
)";

int main() {
  auto verts = std::vector<glm::vec3>();
  auto tris = std::vector<glm::ivec3>();

  verts.push_back(glm::vec3(-1.0, -1.0, 1.0));
  verts.push_back(glm::vec3(1.0, -1.0, 1.0));
  verts.push_back(glm::vec3(1.0, 1.0, 1.0));
  verts.push_back(glm::vec3(-1.0, 1.0, 1.0));
  verts.push_back(glm::vec3(-1.0, -1.0, -1.0));
  verts.push_back(glm::vec3(1.0, -1.0, -1.0));
  verts.push_back(glm::vec3(1.0, 1.0, -1.0));
  verts.push_back(glm::vec3(-1.0, 1.0, -1.0));

  tris.push_back(glm::ivec3(0, 1, 2));
  tris.push_back(glm::ivec3(2, 3, 0));
  tris.push_back(glm::ivec3(1, 5, 6));
  tris.push_back(glm::ivec3(6, 2, 1));
  tris.push_back(glm::ivec3(7, 6, 5));
  tris.push_back(glm::ivec3(5, 4, 7));
  tris.push_back(glm::ivec3(4, 0, 3));
  tris.push_back(glm::ivec3(3, 7, 4));
  tris.push_back(glm::ivec3(4, 5, 1));
  tris.push_back(glm::ivec3(1, 0, 4));
  tris.push_back(glm::ivec3(3, 2, 6));
  tris.push_back(glm::ivec3(6, 7, 3));

  auto mesh = gle::Mesh(verts, tris);
  auto window = gle::Window("Basic Example", 720, 480);
  auto shader = gle::Shader(vert, frag);
  window.init();
  shader.load();
  shader.use();
  window.start();
}