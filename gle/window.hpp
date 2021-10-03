#ifndef GLE_WINDOW_HPP
#define GLE_WINDOW_HPP

#include <exception>
#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <gle/logging.hpp>
#include <glm/glm.hpp>
#include <string>

GLE_NAMESPACE_BEGIN

namespace __internal__ {
inline void framebuffer_callback(GLFWwindow *window, int width, int height);
}

struct GleWindowOptions {
  int gl_num_samples = 4;
  int gl_major_version = 4;
  int gl_minor_version = 1;
};

class GleWindow {
public:
  GleWindow(GleWindow &) = delete;
  GleWindow(GleWindow &&) = delete;
  GleWindow(const GleWindow &) = delete;
  GleWindow(const GleWindow &&) = delete;
  inline GleWindow(const std::string &name, const glm::ivec2 &dimensions);
  inline GleWindow(const std::string &name, int width, int height);
  inline GleWindow(const std::string &name, GleWindowOptions options,
                   const glm::ivec2 &dimensions);
  inline GleWindow(const std::string &name, GleWindowOptions options, int width,
                   int height);

  inline ~GleWindow();

  inline void init();

  // Getters and setters
  constexpr GleWindowOptions &options();
  constexpr const GleWindowOptions &options() const;
  constexpr const std::string &name() const;
  constexpr const glm::ivec2 &dimensions() const;
  constexpr int width() const;
  constexpr int height() const;

private:
  std::string _name;
  glm::ivec2 _dimensions;
  GleWindowOptions _options;

  GLFWwindow *window = nullptr;
  friend inline void __internal__::framebuffer_callback(GLFWwindow *window,
                                                        int width, int height);
};

GLE_NAMESPACE_END

#include "window.inl"

#endif // GLE_WINDOW_HPP
