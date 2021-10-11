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

struct RenderLoopTask {
  inline virtual void update();
  inline virtual ~RenderLoopTask();
};

struct KeyboardListener {
  inline virtual void key_press(int key, int mods);
  inline virtual void key_repeat(int key, int mods);
  inline virtual void key_release(int key, int mods);
  inline virtual ~KeyboardListener();
};

struct MouseListener {
  inline virtual void mouse_press(int key, int mods, double x, double y);
  inline virtual void mouse_release(int key, int mods, double x, double y);
  inline virtual void mouse_move(double x, double y);
  inline virtual ~MouseListener();
};

namespace __internal__ {
inline void framebuffer_callback(GLFWwindow *window, int width, int height);
inline void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
inline void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods);
inline void mouse_move_callback(GLFWwindow *window, double x, double y);
} // namespace __internal__

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
  inline void init(Scene &scene);

  /// @brief Start the window rendering loop
  ///
  inline void start(const Scene &scene);

  template <class T, class... Args>
  inline RenderPass &make_render_pass(Args &&...args);

  inline void add_keyboard_listener(KeyboardListener &listener);

  inline void add_mouse_listener(MouseListener &listener);

  inline void add_task(RenderLoopTask &task);

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

#ifdef DEBUG_TIMER
  inline double average_frame_time() const;
#endif

private:
  std::string _name;
  glm::ivec2 _dimensions;
  WindowOptions _options;
  std::vector<std::unique_ptr<RenderPass>> render_passes;
  std::vector<KeyboardListener *> keyboard_listeners;
  std::vector<MouseListener *> mouse_listeners;
  std::vector<RenderLoopTask *> render_loop_tasks;
#ifdef DEBUG_TIMER
  std::size_t frames_rendered = 0;
  double frame_time = 0;
#endif

  GLFWwindow *_window = nullptr;
  friend inline void __internal__::framebuffer_callback(GLFWwindow *window,
                                                        int width, int height);
  friend inline void __internal__::key_callback(GLFWwindow *window, int key,
                                                int scancode, int action,
                                                int mods);
  friend inline void __internal__::mouse_button_callback(GLFWwindow *window,
                                                         int button, int action,
                                                         int mods);
  friend inline void __internal__::mouse_move_callback(GLFWwindow *window,
                                                       double x, double y);
};

GLE_NAMESPACE_END

#endif // GLE_WINDOW_HPP
