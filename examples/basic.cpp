#define GLE_VERBOSE
#include <gle/gle.hpp>
#include <iostream>

int main() {
  auto verts = std::vector<glm::vec3>();
  auto tris = std::vector<glm::uvec3>();

  verts.push_back(glm::vec3(-1.0, -1.0, 1.0));
  verts.push_back(glm::vec3(1.0, -1.0, 1.0));
  verts.push_back(glm::vec3(1.0, 1.0, 1.0));
  verts.push_back(glm::vec3(-1.0, 1.0, 1.0));
  verts.push_back(glm::vec3(-1.0, -1.0, -1.0));
  verts.push_back(glm::vec3(1.0, -1.0, -1.0));
  verts.push_back(glm::vec3(1.0, 1.0, -1.0));
  verts.push_back(glm::vec3(-1.0, 1.0, -1.0));

  tris.push_back(glm::uvec3(0, 1, 2));
  tris.push_back(glm::uvec3(2, 3, 0));
  tris.push_back(glm::uvec3(1, 5, 6));
  tris.push_back(glm::uvec3(6, 2, 1));
  tris.push_back(glm::uvec3(7, 6, 5));
  tris.push_back(glm::uvec3(5, 4, 7));
  tris.push_back(glm::uvec3(4, 0, 3));
  tris.push_back(glm::uvec3(3, 7, 4));
  tris.push_back(glm::uvec3(4, 5, 1));
  tris.push_back(glm::uvec3(1, 0, 4));
  tris.push_back(glm::uvec3(3, 2, 6));
  tris.push_back(glm::uvec3(6, 7, 3));

  auto window = gle::Window("Basic Example", 720, 480);

  auto cube_shader =
      std::make_shared<gle::SolidColorShader>(glm::vec3(1, 0, 0));
  auto cube_mesh = std::make_shared<gle::Mesh>(verts, tris);
  auto cube_object = std::make_shared<gle::Object>(cube_shader, cube_mesh);

  auto render_pass = std::make_shared<gle::ObjectRenderPass>();
  render_pass->add_object(cube_object);

  auto camera = std::make_shared<gle::Camera>(
      glm::vec3(3, 3, 3), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0),
      720.0f / 480.0f, glm::radians(45.0f), 0.1f, 100.0f);

  window.add_render_pass(render_pass);
  window.set_camera(camera);

  window.init();
  cube_shader->load();
  window.start();
}