#define GLE_VERBOSE
#include <chrono>
#include <fstream>
#include <gle/gle.hpp>
#include <iostream>
#include <thread>

int main() {

  auto scene = std::make_shared<gle::Scene>();

  auto solid_shader = std::make_shared<gle::SolidColorShader>();
  auto standard_shader = std::make_shared<gle::StandardShader>();

  auto brick_color_texture = std::make_shared<gle::ImageTexture>();
  auto brick_normal_texture = std::make_shared<gle::ImageTexture>();

  auto brick_material = std::make_shared<gle::StandardMaterial>(
      brick_color_texture, brick_normal_texture, 1.0, 1.0);

  auto white_material = std::make_shared<gle::SolidColorMaterial>(
      glm::vec3(0.8, 0.8, 0.8), 1.0, 1.0);

  auto red_material = std::make_shared<gle::SolidColorMaterial>(
      glm::vec3(1.0, 0.0, 0.0), 1.0, 1.0);
  auto green_material = std::make_shared<gle::SolidColorMaterial>(
      glm::vec3(0.0, 1.0, 0.0), 1.0, 1.0);
  auto blue_material = std::make_shared<gle::SolidColorMaterial>(
      glm::vec3(0.0, 0.0, 1.0), 1.0, 1.0);
  auto yellow_material = std::make_shared<gle::SolidColorMaterial>(
      glm::vec3(1.0, 1.0, 0.0), 1.0, 1.0);

  auto obj_mesh = gle::load_obj_from_file("examples/teacup.obj");
  auto obj_object = std::make_shared<gle::Object>(
      solid_shader, white_material, obj_mesh, glm::vec3(-3, 0, -2),
      glm::vec3(0, 0, 0), glm::vec3(1));

  auto sphere_mesh = gle::make_ico_sphere_mesh(3);
  auto sphere_object = std::make_shared<gle::Object>(
      standard_shader, brick_material, sphere_mesh, glm::vec3(-1, 1, 1),
      glm::vec3(0), glm::vec3(0.7));

  auto plane_mesh = gle::make_plane_mesh(5);
  auto plane_object = std::make_shared<gle::Object>(
      standard_shader, brick_material, plane_mesh, glm::vec3(-10, 0, -10),
      glm::vec3(0), glm::vec3(20));

  auto arrow_mesh = gle::make_arrow(1);
  auto y_arrow_object = std::make_shared<gle::Object>(
      solid_shader, green_material, arrow_mesh, glm::vec3(0, 0, 0),
      glm::vec3(0, 0, 0), glm::vec3(1));
  auto x_arrow_object = std::make_shared<gle::Object>(
      solid_shader, red_material, arrow_mesh, glm::vec3(0, 0, 0),
      glm::vec3(0, 0, -M_PI / 2), glm::vec3(1));
  auto z_arrow_object = std::make_shared<gle::Object>(
      solid_shader, blue_material, arrow_mesh, glm::vec3(0, 0, 0),
      glm::vec3(M_PI / 2, 0, 0), glm::vec3(1));

  auto main_light =
      std::make_shared<gle::Light>(gle::DIRECTIONAL_LIGHT, glm::vec3(0, 0, 0),
                                   glm::vec3(-1, -1, -1), glm::vec3(1), 1.0);

  auto other_light = std::make_shared<gle::Light>(
      gle::POINT_LIGHT, glm::vec3(-3, 0, -4), glm::vec3(-5, 5, -5),
      glm::vec3(0.3, 0.8, 1.0), 3.0);
  scene->add_object(sphere_object);
  scene->add_object(plane_object);
  scene->add_object(obj_object);
  scene->add_object(x_arrow_object);
  scene->add_object(y_arrow_object);
  scene->add_object(z_arrow_object);
  scene->add_light(main_light);
  // scene->add_light(other_light);

  auto camera = std::make_shared<gle::Camera>(
      glm::vec3(10, 5, 10), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0),
      720.0f / 480.0f, glm::radians(45.0f), 0.1f, 100.0f);

  scene->camera(camera);

  auto window = gle::Window("Basic Example", 720, 480);

  auto shadow_pass = std::make_shared<gle::ShadowRenderPass>();
  auto render_pass = std::make_shared<gle::ObjectRenderPass>();
  window.add_render_pass(shadow_pass);
  window.add_render_pass(render_pass);

  window.init(scene);
  brick_color_texture->init();
  brick_color_texture->load("examples/col.jpg");
  brick_normal_texture->init();
  brick_normal_texture->load("examples/norm.jpg");

  auto do_animation = [](std::shared_ptr<gle::Camera> camera,
                         std::shared_ptr<gle::Object> object) {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      camera->origin(glm::vec3(sin(glfwGetTime() / 2.0) * 10.0, 5.0,
                               cos(glfwGetTime() / 2.0) * 10.0));
      auto pos = object->position();
      pos.y = sin(glfwGetTime() * 1.37) + 1.0;
      object->position(pos);
    }
  };

  auto animation_thread = std::thread(do_animation, camera, sphere_object);
  animation_thread.detach();
  window.start(scene);
}