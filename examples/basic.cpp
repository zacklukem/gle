#define GLE_VERBOSE
#include <gle/gle.hpp>

int main() {
  auto window = gle::Window("Basic Example", 720, 480);
  window.init();
}