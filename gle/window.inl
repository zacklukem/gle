GLE_NAMESPACE_BEGIN

namespace __internal__ {
inline void framebuffer_callback(GLFWwindow *window, int width, int height) {
  auto self = (Window *)glfwGetWindowUserPointer(window);
  GLE_LOG(GLE_INFO, "Window resize: W: %d, H: %d", width, height);
  self->_dimensions.x = width;
  self->_dimensions.y = height;
  glViewport(0, 0, width, height);
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
      pass->do_render(scene);
    }

    glfwSwapBuffers(window());
    glfwPollEvents();
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

GLE_NAMESPACE_END