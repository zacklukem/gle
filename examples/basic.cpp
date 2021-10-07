#define GLE_VERBOSE
#include <fstream>
#include <gle/gle.hpp>
#include <iostream>

int main() {

  std::fstream teacup("examples/teacup.obj", std::ios_base::in);

  if (!teacup) throw std::runtime_error("teacup.obj not found");

  auto window = gle::Window("Basic Example", 720, 480);

  auto solid_shader = std::make_shared<gle::SolidColorShader>();

  auto green_material =
      std::make_shared<gle::SolidColorMaterial>(glm::vec3(0, 0, 1));

  auto red_material =
      std::make_shared<gle::SolidColorMaterial>(glm::vec3(1, 0, 0));

  auto obj_mesh = gle::load_obj(teacup);
  auto obj_object = std::make_shared<gle::Object>(
      solid_shader, red_material, obj_mesh, glm::vec3(-3, -1, -2),
      glm::vec3(0.2, 0.3, 0.1), glm::vec3(1));

  auto sphere_mesh = gle::make_ico_sphere_mesh(3);
  auto sphere_object = std::make_shared<gle::Object>(
      solid_shader, green_material, sphere_mesh, glm::vec3(3, 1, 2),
      glm::vec3(0), glm::vec3(0.7));

  auto render_pass = std::make_shared<gle::ObjectRenderPass>();
  render_pass->add_object(sphere_object);
  render_pass->add_object(obj_object);

  auto camera = std::make_shared<gle::Camera>(
      glm::vec3(5, 5, 5), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0),
      720.0f / 480.0f, glm::radians(45.0f), 0.1f, 100.0f);

  window.add_render_pass(render_pass);
  window.set_camera(camera);

  window.init();
  solid_shader->load();
  window.start();
}