GLE_NAMESPACE_BEGIN

namespace __internal__ {
inline void framebuffer_callback(GLFWwindow *window, int width, int height) {
  auto self = (Window *)glfwGetWindowUserPointer(window);
  GLE_LOG(GLE_INFO, "Window resize: W: %d, H: %d", width, height);
  self->_dimensions.x = width;
  self->_dimensions.y = height;
  glViewport(0, 0, width, height);
}

inline void key_callback(GLFWwindow *window, int key, int, int action,
                         int mods) {
  auto self = (Window *)glfwGetWindowUserPointer(window);
  switch (action) {
  case GLFW_PRESS:
    for (auto listener : self->keyboard_listeners) {
      listener->key_press(key, mods);
    }
    break;
  case GLFW_REPEAT:
    for (auto listener : self->keyboard_listeners) {
      listener->key_repeat(key, mods);
    }
    break;
  case GLFW_RELEASE:
    for (auto listener : self->keyboard_listeners) {
      listener->key_release(key, mods);
    }
    break;
  }
}

inline void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods) {
  auto self = (Window *)glfwGetWindowUserPointer(window);
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  switch (action) {
  case GLFW_PRESS:
    for (auto listener : self->mouse_listeners) {
      listener->mouse_press(button, mods, x, y);
    }
    break;
  case GLFW_RELEASE:
    for (auto listener : self->mouse_listeners) {
      listener->mouse_release(button, mods, x, y);
    }
    break;
  }
}
inline void mouse_move_callback(GLFWwindow *window, double x, double y) {
  auto self = (Window *)glfwGetWindowUserPointer(window);
  for (auto listener : self->mouse_listeners) {
    listener->mouse_move(x, y);
  }
}

} // namespace __internal__

inline Window::Window(const std::string &name, const glm::ivec2 &dimensions)
    : _name(name), _dimensions(dimensions), render_passes() {}
inline Window::Window(const std::string &name, int width, int height)
    : _name(name), _dimensions(width, height), render_passes() {}

inline Window::Window(const std::string &name, WindowOptions options,
                      const glm::ivec2 &dimensions)
    : _name(name), _dimensions(dimensions), _options(options), render_passes() {
}
inline Window::Window(const std::string &name, WindowOptions options, int width,
                      int height)
    : _name(name), _dimensions(width, height), _options(options),
      render_passes() {}

inline Window::~Window() {
  glfwDestroyWindow(window());
  glfwTerminate();
}

inline void Window::init(std::shared_ptr<Scene> scene) {
  glfwInit();
  glfwWindowHint(GLFW_SAMPLES, options().gl_num_samples);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options().gl_major_version);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options().gl_minor_version);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  _window =
      glfwCreateWindow(width(), height(), name().c_str(), nullptr, nullptr);

  if (window() == nullptr) {
    const char *description;
    glfwGetError(&description);
    throw std::runtime_error(std::string("Failed to create GLFW window") +
                             description);
  }

  int actual_width;
  int actual_height;
  glfwGetFramebufferSize(window(), &actual_width, &actual_height);
  if (actual_width != width() || actual_height != height()) {
    GLE_LOG(GLE_WARN,
            "Unexpected framebuffer size: W: %d, H: %d, expected: W: %d, H: %d",
            actual_width, actual_height, width(), height());
    _dimensions.x = actual_width;
    _dimensions.y = actual_height;
  }

  glfwSetWindowUserPointer(window(), this);
  glfwMakeContextCurrent(window());
  glfwSetFramebufferSizeCallback(window(), __internal__::framebuffer_callback);
  glfwSetKeyCallback(window(), __internal__::key_callback);
  glfwSetMouseButtonCallback(window(), __internal__::mouse_button_callback);
  glfwSetCursorPosCallback(window(), __internal__::mouse_move_callback);

#ifdef GLE_VERBOSE
  const char *glfw_version = glfwGetVersionString();
  GLE_LOG(GLE_INFO, "GLFW version %s loaded.", glfw_version);
#endif

  // load GL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to load GLAD");

    // GL is now loaded

#ifdef GLE_VERBOSE
  const unsigned char *version = glGetString(GL_VERSION);
  GLE_LOG(GLE_INFO, "OpenGl version %sloaded.", version);
#endif

  glViewport(0, 0, width(), height());
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_FRAMEBUFFER_SRGB);

  scene->init();

  for (auto pass : render_passes) {
    pass->load(scene);
  }
}

inline void Window::start(std::shared_ptr<const Scene> scene) {
  while (!glfwWindowShouldClose(window())) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &pass : render_passes) {
      glViewport(0, 0, width(), height());
      pass->do_render(scene);
    }

    glfwSwapBuffers(window());
    glfwPollEvents();
    for (auto task : render_loop_tasks) {
      task->update();
    }
  }
}

inline void Window::add_render_pass(std::shared_ptr<RenderPass> pass) {
  render_passes.push_back(pass);
}

constexpr WindowOptions &Window::options() { return _options; }
constexpr const WindowOptions &Window::options() const { return _options; }

constexpr const std::string &Window::name() const { return _name; }

constexpr const glm::ivec2 &Window::dimensions() const { return _dimensions; }

constexpr int Window::width() const { return _dimensions.x; }
constexpr int Window::height() const { return _dimensions.y; }

constexpr GLFWwindow *Window::window() { return _window; }

inline void KeyboardListener::key_press(int, int) {}
inline void KeyboardListener::key_repeat(int, int) {}
inline void KeyboardListener::key_release(int, int) {}

inline void
Window::add_keyboard_listener(std::shared_ptr<KeyboardListener> listener) {
  keyboard_listeners.push_back(listener);
}

inline void
Window::add_mouse_listener(std::shared_ptr<MouseListener> listener) {
  mouse_listeners.push_back(listener);
}

inline void Window::add_task(std::shared_ptr<RenderLoopTask> task) {
  render_loop_tasks.push_back(task);
}

inline KeyboardListener::~KeyboardListener() {}

inline void MouseListener::mouse_press(int, int, double, double) {}
inline void MouseListener::mouse_release(int, int, double, double) {}
inline void MouseListener::mouse_move(double, double) {}
inline MouseListener::~MouseListener() {}

inline void RenderLoopTask::update() {}
inline RenderLoopTask::~RenderLoopTask() {}

GLE_NAMESPACE_END