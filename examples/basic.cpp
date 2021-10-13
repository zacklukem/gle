#define GLE_VERBOSE
#define DEBUG_TIMER
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
    RIGHT = 0x8,
    UP = 0x10,
    DOWN = 0x20
  };

private:
  gle::Camera &camera;
  uint8_t state = State::NONE;
  bool mouse_down = false;
  double ox, oy;
  int updates = 0;

public:
  CameraStateController(gle::Camera &camera) : camera(camera) {}

  void update() override {
    if (state != State::NONE) {
      auto right = glm::cross(camera.direction(), glm::vec3(0, 1, 0));
      auto cam_up = glm::cross(camera.direction(), right);
      glm::vec3 movement = glm::vec3(0);
      if (state & State::REVERSE) {
        movement -= camera.direction() * 0.3f;
      }
      if (state & State::FORWARD) {
        movement += camera.direction() * 0.3f;
      }
      if (state & State::RIGHT) {
        movement += right * 0.3f;
      }
      if (state & State::LEFT) {
        movement -= right * 0.3f;
      }
      if (state & State::UP) {
        movement -= cam_up * 0.3f;
      }
      if (state & State::DOWN) {
        movement += cam_up * 0.3f;
      }
      camera.origin(camera.origin() + movement);
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
    else if (key == GLFW_KEY_SPACE)
      state |= State::UP;
    else if (key == GLFW_KEY_LEFT_SHIFT)
      state |= State::DOWN;
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
    else if (key == GLFW_KEY_SPACE)
      state &= ~State::UP;
    else if (key == GLFW_KEY_LEFT_SHIFT)
      state &= ~State::DOWN;
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
        glm::normalize(glm::cross(camera.direction(), glm::vec3(0, 1, 0)));

    auto direction = camera.direction();

    direction = glm::rotate(direction, dx, glm::vec3(0.0f, 1.0f, 0.0f));
    direction = glm::rotate(direction, dy, right);

    camera.direction(direction);

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

  auto scene = gle::Scene();

  auto solid_shader = gle::SolidColorShader();
  auto standard_shader = gle::StandardShader();

  auto stone_color_texture = gle::ImageTexture("examples/1K/COL_1K.jpg");
  auto stone_normal_texture = gle::ImageTexture("examples/1K/NRM_1K.jpg");
  auto stone_disp_texture = gle::ImageTexture("examples/1K/DISP_1K.jpg");

  auto stone_material =
      gle::StandardMaterial(stone_color_texture, stone_normal_texture,
                            stone_disp_texture, 0.03, 1.0, 1.0);

  auto white_material =
      gle::SolidColorMaterial(glm::vec3(0.8, 0.8, 0.8), 1.0, 1.0);

  auto red_material =
      gle::SolidColorMaterial(glm::vec3(1.0, 0.0, 0.0), 1.0, 1.0);
  auto green_material =
      gle::SolidColorMaterial(glm::vec3(0.0, 1.0, 0.0), 1.0, 1.0);
  auto blue_material =
      gle::SolidColorMaterial(glm::vec3(0.0, 0.0, 1.0), 1.0, 1.0);
  auto yellow_material =
      gle::SolidColorMaterial(glm::vec3(1.0, 1.0, 0.0), 1.0, 1.0);

  auto obj_mesh = gle::load_obj_from_file("examples/teacup.obj");
  scene.make_object(solid_shader, white_material, *obj_mesh,
                    glm::vec3(-3, 0, -2), glm::vec3(0, 0, 0), glm::vec3(1));

  auto sphere_mesh = gle::make_ico_sphere_mesh(3);
  auto &sphere_object =
      scene.make_object(standard_shader, stone_material, *sphere_mesh,
                        glm::vec3(-1, 1, 1), glm::vec3(0), glm::vec3(0.7));

  auto plane_mesh = gle::make_plane_mesh(5);
  scene.make_object(standard_shader, stone_material, *plane_mesh,
                    glm::vec3(-10, 0, -10), glm::vec3(0), glm::vec3(20));

  auto arrow_mesh = gle::make_arrow(1);
  scene.make_object(solid_shader, green_material, *arrow_mesh,
                    glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));
  scene.make_object(solid_shader, red_material, *arrow_mesh, glm::vec3(0, 0, 0),
                    glm::vec3(0, 0, -M_PI / 2), glm::vec3(1));
  scene.make_object(solid_shader, blue_material, *arrow_mesh,
                    glm::vec3(0, 0, 0), glm::vec3(M_PI / 2, 0, 0),
                    glm::vec3(1));

  scene.make_light(gle::DIRECTIONAL_LIGHT, glm::vec3(0, 0, 0),
                   glm::vec3(-1, -1, -1), glm::vec3(1), 1.0);

  auto &camera = scene.make_camera(glm::vec3(10, 5, 10), glm::vec3(0, 1, 0),
                                   glm::vec3(-10, -5, -10), 720.0f / 480.0f,
                                   glm::radians(45.0f), 0.1f, 100.0f);

  auto window = gle::Window("Basic Example", 720, 480);

  auto camera_state = CameraStateController(camera);
  window.add_keyboard_listener(camera_state);
  window.add_mouse_listener(camera_state);
  window.add_task(camera_state);

  window.make_render_pass<gle::ShadowRenderPass>();
  window.make_render_pass<gle::ObjectRenderPass>();

  window.init(scene);
  stone_color_texture.init();
  stone_color_texture.load();
  stone_normal_texture.init();
  stone_normal_texture.load();
  stone_disp_texture.init();
  stone_disp_texture.load();

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