#include "camera.hpp"
#include <toml_scene.hpp>

int main() {
  auto window = gle::Window("Toml Scene Example", 720, 480);

  auto scene = toml_scene::make_scene();
  auto camera_controller = CameraController(scene->camera());

  window.add_keyboard_listener(camera_controller);
  window.add_mouse_listener(camera_controller);
  window.add_task(camera_controller);

  window.make_render_pass<gle::ShadowRenderPass>();
  window.make_render_pass<gle::ObjectRenderPass>();

  window.init(*scene);
  window.start(*scene);
  return 0;
}