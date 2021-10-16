#define GLE_VERBOSE
#define DEBUG_TIMER
#include "camera.hpp"
#include <chrono>
#include <fstream>
#include <gle/gle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <thread>

int main() {

  auto scene = gle::Scene();

  auto &solid_shader = scene.make_shader<gle::SolidColorShader>();
  auto &standard_shader = scene.make_shader<gle::StandardShader>();

  auto &stone_color_texture =
      scene.make_texture<gle::ImageTexture>("res/COL_1K.jpg");
  auto &stone_normal_texture =
      scene.make_texture<gle::ImageTexture>("res/NRM_1K.jpg");
  auto &stone_disp_texture =
      scene.make_texture<gle::ImageTexture>("res/DISP_1K.jpg");

  auto &stone_material = scene.make_material<gle::StandardMaterial>(
      stone_color_texture, stone_normal_texture, stone_disp_texture, 0.03, 1.0,
      1.0);

  auto &white_material = scene.make_material<gle::SolidColorMaterial>(
      glm::vec3(0.8, 0.8, 0.8), 1.0, 1.0);

  auto &red_material = scene.make_material<gle::SolidColorMaterial>(
      glm::vec3(1.0, 0.0, 0.0), 1.0, 1.0);
  auto &green_material = scene.make_material<gle::SolidColorMaterial>(
      glm::vec3(0.0, 1.0, 0.0), 1.0, 1.0);
  auto &blue_material = scene.make_material<gle::SolidColorMaterial>(
      glm::vec3(0.0, 0.0, 1.0), 1.0, 1.0);

  auto &obj_mesh = scene.mesh(gle::load_obj_from_file("res/teacup.obj"));
  scene.make_object(solid_shader, white_material, obj_mesh,
                    glm::vec3(-3, 0, -2), glm::vec3(0, 0, 0), glm::vec3(1));

  auto &sphere_mesh = scene.mesh(gle::make_ico_sphere_mesh(3));
  auto &sphere_object =
      scene.make_object(standard_shader, stone_material, sphere_mesh,
                        glm::vec3(-1, 1, 1), glm::vec3(0), glm::vec3(0.7));

  auto &plane_mesh = scene.mesh(gle::make_plane_mesh(5));
  scene.make_object(standard_shader, stone_material, plane_mesh,
                    glm::vec3(-10, 0, -10), glm::vec3(0), glm::vec3(20));

  auto &arrow_mesh = scene.mesh(gle::make_arrow(1));
  scene.make_object(solid_shader, green_material, arrow_mesh,
                    glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));
  scene.make_object(solid_shader, red_material, arrow_mesh, glm::vec3(0, 0, 0),
                    glm::vec3(0, 0, -M_PI / 2), glm::vec3(1));
  scene.make_object(solid_shader, blue_material, arrow_mesh, glm::vec3(0, 0, 0),
                    glm::vec3(M_PI / 2, 0, 0), glm::vec3(1));

  scene.make_light(gle::DIRECTIONAL_LIGHT, glm::vec3(0, 0, 0),
                   glm::vec3(-1, -1, -1), glm::vec3(1), 1.0);

  auto &camera = scene.make_camera(glm::vec3(10, 5, 10), glm::vec3(0, 1, 0),
                                   glm::vec3(-10, -5, -10), 720.0f / 480.0f,
                                   glm::radians(45.0f), 0.1f, 100.0f);

  auto window = gle::Window("Basic Example", 720, 480);

  auto camera_state = CameraController(camera);
  window.add_keyboard_listener(camera_state);
  window.add_mouse_listener(camera_state);
  window.add_task(camera_state);

  window.make_render_pass<gle::ShadowRenderPass>();
  window.make_render_pass<gle::ObjectRenderPass>();

  window.init(scene);

  auto do_animation = [](gle::Object *object) {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      auto pos = object->position();
      pos.y = sin(glfwGetTime() * 1.37) + 1.0;
      object->position(pos);
    }
  };

  auto animation_thread = std::thread(do_animation, &sphere_object);
  animation_thread.detach();

  window.start(scene);
  std::cout << "Average Frame Time: " << window.average_frame_time() * 1000.0
            << std::endl;
}