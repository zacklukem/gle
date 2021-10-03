#ifndef GLE_WINDOW_HPP
#define GLE_WINDOW_HPP

#include <exception>
#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <gle/logging.hpp>
#include <gle/render_pass.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

GLE_NAMESPACE_BEGIN

namespace __internal__ {
inline void framebuffer_callback(GLFWwindow *window, int width, int height);
}

struct WindowOptions {
  int gl_num_samples = 4;
  int gl_major_version = 4;
  int gl_minor_version = 1;
};

class Window {
public:
  Window(Window &) = delete;
  Window(Window &&) = delete;
  Window(const Window &) = delete;
  Window(const Window &&) = delete;
  inline Window(const std::string &name, const glm::ivec2 &dimensions);
  inline Window(const std::string &name, int width, int height);
  inline Window(const std::string &name, WindowOptions options,
                const glm::ivec2 &dimensions);
  inline Window(const std::string &name, WindowOptions options, int width,
                int height);

  inline ~Window();

  inline void init();
  inline void start();

  // Getters and setters
  inline void add_render_pass(std::shared_ptr<RenderPass> pass);
  constexpr WindowOptions &options();
  constexpr const WindowOptions &options() const;
  constexpr const std::string &name() const;
  constexpr const glm::ivec2 &dimensions() const;
  constexpr int width() const;
  constexpr int height() const;
  constexpr GLFWwindow *window();

private:
  std::string _name;
  glm::ivec2 _dimensions;
  WindowOptions _options;
  std::vector<std::shared_ptr<RenderPass>> render_passes;

  GLFWwindow *_window = nullptr;
  friend inline void __internal__::framebuffer_callback(GLFWwindow *window,
                                                        int width, int height);
};

GLE_NAMESPACE_END

#include "window.inl"

#endif // GLE_WINDOW_HPP
