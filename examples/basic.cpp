#define GLE_VERBOSE
#include <chrono>
#include <fstream>
#include <gle/gle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <thread>

class CameraStateController : public gle::KeyboardListener,
                              public gle::MouseListener,
                              public gle::RenderLoopTask {
private:
  enum State : uint8_t {
    NONE = 0x0,
    FORWARD = 0x1,
    REVERSE = 0x2,
    LEFT = 0x4,
    RIGHT = 0x8
  };

private:
  std::shared_ptr<gle::Camera> camera;
  uint8_t state = State::NONE;
  bool mouse_down = false;
  double ox, oy;
  int updates = 0;

public:
  CameraStateController(std::shared_ptr<gle::Camera> camera) : camera(camera) {}
  void update() override {
    if (state != State::NONE) {
      glm::vec3 movement = glm::vec3(0);
      if (state & State::REVERSE) {
        movement += -camera->direction() * 0.3f;
      }
      if (state & State::FORWARD) {
        movement += camera->direction() * 0.3f;
      }
      if (state & State::RIGHT) {
        auto right = glm::cross(camera->direction(), glm::vec3(0, 1, 0));
        movement += right * 0.3f;
      }
      if (state & State::LEFT) {
        auto right = glm::cross(camera->direction(), glm::vec3(0, 1, 0));
        movement += -right * 0.3f;
      }
      camera->origin(camera->origin() + movement);
    }
  }

  inline virtual void key_press(int key, int) override {
    if (key == GLFW_KEY_W)
      state |= State::FORWARD;
    else if (key == GLFW_KEY_S)
      state |= State::REVERSE;
    else if (key == GLFW_KEY_A)
      state |= State::LEFT;
    else if (key == GLFW_KEY_D)
      state |= State::RIGHT;
  }

  inline virtual void key_release(int key, int) override {
    if (key == GLFW_KEY_W)
      state &= ~State::FORWARD;
    else if (key == GLFW_KEY_S)
      state &= ~State::REVERSE;
    else if (key == GLFW_KEY_A)
      state &= ~State::LEFT;
    else if (key == GLFW_KEY_D)
      state &= ~State::RIGHT;
  }

  inline virtual void mouse_move(double x, double y) override {
    if (!mouse_down) return;
    float dx = ox - x;
    float dy = oy - y;
    updates++;
    if (updates < 2) {
      ox = x;
      oy = y;
      return;
    }
    dx *= 0.03f;
    dy *= 0.03f;

    auto right =
        glm::normalize(glm::cross(camera->direction(), glm::vec3(0, 1, 0)));

    auto direction = camera->direction();

    direction = glm::rotate(direction, dx, glm::vec3(0.0f, 1.0f, 0.0f));
    direction = glm::rotate(direction, dy, right);

    camera->direction(direction);

    ox = x;
    oy = y;
  }

  inline virtual void mouse_press(int, int, double x, double y) override {
    ox = x;
    ox = y;
    mouse_down = true;
    updates = 0;
  }

  inline virtual void mouse_release(int, int, double, double) override {
    mouse_down = false;
    updates = 0;
  }
};

int main() {

  auto scene = std::make_shared<gle::Scene>();

  auto solid_shader = std::make_shared<gle::SolidColorShader>();
  auto standard_shader = std::make_shared<gle::StandardShader>();

  auto stone_color_texture = std::make_shared<gle::ImageTexture>();
  auto stone_normal_texture = std::make_shared<gle::ImageTexture>();
  auto stone_disp_texture = std::make_shared<gle::ImageTexture>();

  auto stone_material = std::make_shared<gle::StandardMaterial>(
      stone_color_texture, stone_normal_texture, stone_disp_texture, 0.03, 1.0,
      1.0);

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
      standard_shader, stone_material, sphere_mesh, glm::vec3(-1, 1, 1),
      glm::vec3(0), glm::vec3(0.7));

  auto plane_mesh = gle::make_plane_mesh(5);
  auto plane_object = std::make_shared<gle::Object>(
      standard_shader, stone_material, plane_mesh, glm::vec3(-10, 0, -10),
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
      glm::vec3(10, 5, 10), glm::vec3(0, 1, 0), glm::vec3(-10, -5, -10),
      720.0f / 480.0f, glm::radians(45.0f), 0.1f, 100.0f);

  scene->camera(camera);

  auto window = gle::Window("Basic Example", 720, 480);

  auto camera_state = std::make_shared<CameraStateController>(camera);
  window.add_keyboard_listener(camera_state);
  window.add_mouse_listener(camera_state);
  window.add_task(camera_state);

  auto shadow_pass = std::make_shared<gle::ShadowRenderPass>();
  auto render_pass = std::make_shared<gle::ObjectRenderPass>();
  window.add_render_pass(shadow_pass);
  window.add_render_pass(render_pass);

  window.init(scene);
  stone_color_texture->init();
  stone_color_texture->load("examples/1K/COL_1K.jpg");
  stone_normal_texture->init();
  stone_normal_texture->load("examples/1K/NRM_1K.jpg");
  stone_disp_texture->init();
  stone_disp_texture->load("examples/1K/DISP_1K.jpg");

  auto do_animation = [](std::shared_ptr<CameraStateController>,
                         std::shared_ptr<gle::Object> object) {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      // cam_state->tick();
      auto pos = object->position();
      pos.y = sin(glfwGetTime() * 1.37) + 1.0;
      object->position(pos);
    }
  };

  auto animation_thread =
      std::thread(do_animation, camera_state, sphere_object);
  animation_thread.detach();
  window.start(scene);
}