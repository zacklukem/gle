#define GLE_VERBOSE
#include <chrono>
#include <fstream>
#include <gle/gle.hpp>
#include <iostream>
#include <thread>

int main() {

  auto window = gle::Window("Basic Example", 720, 480);

  auto solid_shader = std::make_shared<gle::SolidColorShader>();
  auto standard_shader = std::make_shared<gle::StandardShader>();

  auto brick_color_texture = std::make_shared<gle::Texture>();
  auto brick_normal_texture = std::make_shared<gle::Texture>();

  auto brick_material = std::make_shared<gle::StandardMaterial>(
      brick_color_texture, brick_normal_texture, 1.0, 1.0);

  auto red_material = std::make_shared<gle::SolidColorMaterial>(
      glm::vec3(0.8, 0.8, 0.8), 1.0, 1.0);

  auto obj_mesh = gle::load_obj_from_file("examples/teacup.obj");
  auto obj_object = std::make_shared<gle::Object>(
      solid_shader, red_material, obj_mesh, glm::vec3(-3, 0, -2),
      glm::vec3(0, 0, 0), glm::vec3(1));

  auto sphere_mesh = gle::make_ico_sphere_mesh(3);
  auto sphere_object = std::make_shared<gle::Object>(
      standard_shader, brick_material, sphere_mesh, glm::vec3(3, 1, 2),
      glm::vec3(0), glm::vec3(0.7));

  auto plane_mesh = gle::make_plane_mesh(5);
  auto plane_object = std::make_shared<gle::Object>(
      standard_shader, brick_material, plane_mesh, glm::vec3(-10, 0, -10),
      glm::vec3(0), glm::vec3(20));

  auto main_light =
      std::make_shared<gle::Light>(gle::DIRECTIONAL_LIGHT, glm::vec3(0),
                                   glm::vec3(5, 10, 5), glm::vec3(1), 1.0);

  auto other_light = std::make_shared<gle::Light>(
      gle::POINT_LIGHT, glm::vec3(-3, 0, -4), glm::vec3(-5, 5, -5),
      glm::vec3(0.3, 0.8, 1.0), 3.0);
  auto render_pass = std::make_shared<gle::ObjectRenderPass>();
  render_pass->add_object(sphere_object);
  render_pass->add_object(plane_object);
  render_pass->add_object(obj_object);
  render_pass->add_light(main_light);
  render_pass->add_light(other_light);

  auto camera = std::make_shared<gle::Camera>(
      glm::vec3(10, 5, 10), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0),
      720.0f / 480.0f, glm::radians(45.0f), 0.1f, 100.0f);

  auto do_animation = [](std::shared_ptr<gle::Camera> camera) {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      camera->origin(glm::vec3(sin(glfwGetTime() / 2.0) * 10.0, 5.0,
                               cos(glfwGetTime() / 2.0) * 10.0));
    }
  };

  auto animation_thread = std::thread(do_animation, camera);
  animation_thread.detach();

  window.add_render_pass(render_pass);
  window.set_camera(camera);

  window.init();
  solid_shader->load();
  standard_shader->load();
  brick_color_texture->load("examples/col.jpg");
  brick_normal_texture->load("examples/norm.jpg");

  window.start();
}