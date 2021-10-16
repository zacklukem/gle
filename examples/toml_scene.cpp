#include <toml_scene.hpp>

int main() {
  auto window = gle::Window("Toml Scene Example", 720, 480);

  auto scene = make_scene();

  window.make_render_pass<gle::ShadowRenderPass>();
  window.make_render_pass<gle::ObjectRenderPass>();

  window.init(*scene);
  window.start(*scene);
  return 0;
}