#ifndef EXAMPLES_CAMERA_HPP
#define EXAMPLES_CAMERA_HPP

#include <gle/gle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

class CameraController : public gle::KeyboardListener,
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
  CameraController(gle::Camera &camera) : camera(camera) {}

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

#endif // EXAMPLES_CAMERA_HPP
