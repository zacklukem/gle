#ifndef GLE_WINDOW_HPP
#define GLE_WINDOW_HPP

#include <exception>
#include <gle/camera.hpp>
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

/// @brief Window options
///
struct WindowOptions {
  /// @brief Antialiasing samples. Default: x4
  ///
  int gl_num_samples = 4;

  /// @brief OpenGL major version
  ///
  int gl_major_version = 4;

  /// @brief OpenGL minor version
  ///
  int gl_minor_version = 1;
};

/// @brief Representation of the graphics window
///
/// Internally, The window class is a high-level wrapper around the GLFWwindow.
class Window {
public:
  Window(Window &) = delete;
  Window(Window &&) = delete;
  Window(const Window &) = delete;
  Window(const Window &&) = delete;

  /// @brief Construct a new Window object
  ///
  /// @param name
  /// @param dimensions
  inline Window(const std::string &name, const glm::ivec2 &dimensions);

  /// @brief Construct a new Window object
  ///
  /// @param name
  /// @param width
  /// @param height
  inline Window(const std::string &name, int width, int height);

  /// @brief Construct a new Window object
  ///
  /// @param name
  /// @param options
  /// @param dimensions
  inline Window(const std::string &name, WindowOptions options,
                const glm::ivec2 &dimensions);

  /// @brief Construct a new Window object
  ///
  /// @param name
  /// @param options
  /// @param width
  /// @param height
  inline Window(const std::string &name, WindowOptions options, int width,
                int height);

  inline ~Window();

  /// @brief Initialize the window and enable opengl
  ///
  /// @exception std::runtime_error thrown if opengl or glfw fail to load
  inline void init(std::shared_ptr<Scene> scene);

  /// @brief Start the window rendering loop
  ///
  inline void start(std::shared_ptr<const Scene> scene);

  // Getters and setters
  /// @brief Add a rendering pass to the end of the render past list
  ///
  /// @param pass
  inline void add_render_pass(std::shared_ptr<RenderPass> pass);

  /// @brief Get the window options structure
  ///
  /// @return the window options structure
  constexpr WindowOptions &options();

  /// @brief Get the window options structure
  ///
  /// @return the window options structure
  constexpr const WindowOptions &options() const;

  /// @brief Get the window title
  ///
  /// @return the window title
  constexpr const std::string &name() const;

  /// @brief Get the window dimensions vector
  ///
  /// @return the window dimensions vector
  constexpr const glm::ivec2 &dimensions() const;

  /// @brief Get the window width
  ///
  /// @return width of the window
  constexpr int width() const;

  /// @brief Get the window height
  ///
  /// @return height of the window
  constexpr int height() const;

  /// @brief Get the glfw window
  ///
  /// @return the internal glfw window
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
