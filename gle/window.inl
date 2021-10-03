GLE_NAMESPACE_BEGIN

namespace __internal__ {
inline void framebuffer_callback(GLFWwindow *window, int width, int height) {
  auto self = (GleWindow *)glfwGetWindowUserPointer(window);
  self->_dimensions.x = width;
  self->_dimensions.y = height;
  glViewport(0, 0, width, height);
}
} // namespace __internal__

inline GleWindow::GleWindow(const std::string &name,
                            const glm::ivec2 &dimensions)
    : _name(name), _dimensions(dimensions) {}
inline GleWindow::GleWindow(const std::string &name, int width, int height)
    : _name(name), _dimensions(width, height) {}

inline GleWindow::GleWindow(const std::string &name, GleWindowOptions options,
                            const glm::ivec2 &dimensions)
    : _name(name), _options(options), _dimensions(dimensions) {}
inline GleWindow::GleWindow(const std::string &name, GleWindowOptions options,
                            int width, int height)
    : _name(name), _options(options), _dimensions(width, height) {}

inline GleWindow::~GleWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

inline void GleWindow::init() {
  glfwInit();
  glfwWindowHint(GLFW_SAMPLES, options().gl_num_samples);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options().gl_major_version);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options().gl_minor_version);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window =
      glfwCreateWindow(width(), height(), name().c_str(), nullptr, nullptr);

  if (window == nullptr) {
    const char *description;
    int code = glfwGetError(&description);
    throw std::runtime_error(std::string("Failed to create GLFW window") +
                             description);
  }

  glfwSetWindowUserPointer(window, this);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, __internal__::framebuffer_callback);

  // load GL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to load GLAD");

    // GL is now loaded

#ifdef GLE_VERBOSE
  const unsigned char *version = glGetString(GL_VERSION);
  GLE_LOG(GLE_INFO, "OpenGl version %sloaded.", version);
#endif

  glViewport(0, 0, width(), height());
}

constexpr GleWindowOptions &GleWindow::options() { return _options; }
constexpr const GleWindowOptions &GleWindow::options() const {
  return _options;
}

constexpr const std::string &GleWindow::name() const { return _name; }

constexpr const glm::ivec2 &GleWindow::dimensions() const {
  return _dimensions;
}

constexpr int GleWindow::width() const { return _dimensions.x; }
constexpr int GleWindow::height() const { return _dimensions.y; }

GLE_NAMESPACE_END