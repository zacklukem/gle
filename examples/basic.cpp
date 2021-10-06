#define GLE_VERBOSE
#include <gle/gle.hpp>
#include <iostream>

int main() {

  auto window = gle::Window("Basic Example", 720, 480);

  auto red_shader =
      std::make_shared<gle::SolidColorShader>(glm::vec3(1, 0.5, 0.5));
  auto green_shader =
      std::make_shared<gle::SolidColorShader>(glm::vec3(0.5, 1, 0.5));

  auto cube_mesh = gle::make_cube_mesh();
  auto cube_object = std::make_shared<gle::Object>(
      red_shader, cube_mesh, glm::vec3(-3, -1, -2), glm::vec3(0.2, 0.3, 0.1),
      glm::vec3(1));

  auto sphere_mesh = gle::make_ico_sphere_mesh(3);
  auto sphere_object = std::make_shared<gle::Object>(
      green_shader, sphere_mesh, glm::vec3(3, 1, 2), glm::vec3(0),
      glm::vec3(0.7));

  auto render_pass = std::make_shared<gle::ObjectRenderPass>();
  render_pass->add_object(sphere_object);
  render_pass->add_object(cube_object);

  auto camera = std::make_shared<gle::Camera>(
      glm::vec3(5, 5, 5), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0),
      720.0f / 480.0f, glm::radians(45.0f), 0.1f, 100.0f);

  window.add_render_pass(render_pass);
  window.set_camera(camera);

  window.init();
  green_shader->load();
  red_shader->load();
  window.start();
}