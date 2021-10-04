#define GLE_VERBOSE
#include <gle/gle.hpp>
#include <iostream>

int main() {
  auto verts = std::vector<glm::vec3>();
  auto tris = std::vector<glm::uvec3>();

  verts.push_back(glm::vec3(0.5f, 0.5f, 0.0f));
  verts.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
  verts.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
  verts.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));

  tris.push_back(glm::ivec3(0, 1, 3));
  tris.push_back(glm::ivec3(1, 2, 3));

  auto window = gle::Window("Basic Example", 720, 480);

  auto cube_shader = std::make_shared<gle::SolidColorShader>();
  auto cube_mesh = std::make_shared<gle::Mesh>(verts, tris);
  auto cube_object = std::make_shared<gle::Object>(cube_shader, cube_mesh);

  auto render_pass = std::make_shared<gle::ObjectRenderPass>();
  render_pass->add_object(cube_object);

  window.add_render_pass(render_pass);

  window.init();
  cube_shader->load();
  window.start();
}