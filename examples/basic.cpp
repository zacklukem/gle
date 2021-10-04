#define GLE_VERBOSE
#include <gle/gle.hpp>

const char *vert = R"(
#version 410
void main() {}
)";

const char *frag = R"(
#version 410
void main() {}
)";

int main() {
  auto window = gle::Window("Basic Example", 720, 480);
  auto shader = gle::Shader(vert, frag);
  window.init();
  shader.load();
  shader.use();
  window.start();
}