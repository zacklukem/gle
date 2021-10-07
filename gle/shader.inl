
#include <array>
#include <glm/gtc/type_ptr.hpp>

GLE_NAMESPACE_BEGIN

constexpr std::size_t MAX_LIGHTS = 20;

namespace __internal__ {

const char *vertex_default_begin = R"(
#version 410

in vec3 position;
in vec3 normal;
in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
)";

const char *fragment_default_begin = R"(
#version 410

out vec4 FragColor;

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1

struct Light {
  uint type;
  vec3 position;
  vec3 direction;
  vec3 attn;
};

uniform Light lights[20];
uniform uint num_lights;
)";

} // namespace __internal__

inline Shader::Shader(const std::string &vertex_source,
                      const std::string &fragment_source)
    : vertex_source(__internal__::vertex_default_begin + vertex_source),
      fragment_source(__internal__::fragment_default_begin + fragment_source),
      geometry_source(std::nullopt) {}

inline Shader::Shader(const std::string &vertex_source,
                      const std::string &fragment_source,
                      const std::string &geometry_source)
    : vertex_source(__internal__::vertex_default_begin + vertex_source),
      fragment_source(__internal__::fragment_default_begin + fragment_source),
      geometry_source(geometry_source) {}

inline Shader::~Shader() {
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  if (geometry_source.has_value()) glDeleteShader(geometry_shader);
  glDeleteProgram(program);
}

namespace __internal__ {
inline void compile_shader(const std::string &source, GLuint shader) {
  auto c_str_source = source.c_str();
  glShaderSource(shader, 1, &c_str_source, nullptr);
  glCompileShader(shader);
  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled != GL_TRUE) {
    GLchar message[1024];
    glGetShaderInfoLog(shader, 1024, nullptr, message);
    GLE_LOG(GLE_ERR, "shader error: %s", message);
    throw std::runtime_error(message);
  }
}
} // namespace __internal__

inline void Shader::load() {
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  __internal__::compile_shader(vertex_source, vertex_shader);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  __internal__::compile_shader(fragment_source, fragment_shader);

  if (geometry_source.has_value()) {
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    __internal__::compile_shader(geometry_source.value(), geometry_shader);
  }

  program = glCreateProgram();

  glBindAttribLocation(program, 0, "position");
  glBindAttribLocation(program, 1, "normal");
  glBindAttribLocation(program, 2, "uv");

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  if (geometry_source.has_value()) glAttachShader(program, geometry_shader);
  glLinkProgram(program);

  GLint program_linked;
  glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
  if (program_linked != GL_TRUE) {
    GLchar message[1024];
    glGetProgramInfoLog(program, 1024, nullptr, message);
    GLE_LOG(GLE_ERR, "shader error: %s", message);
    throw std::runtime_error(message);
  }
}

inline void Shader::use(const std::vector<std::shared_ptr<Light>> &lights,
                        const MVPShaderUniforms &uniforms,
                        std::shared_ptr<Material> material) {
  glUseProgram(program);
  on_use();
  if (lights.size() > MAX_LIGHTS)
    throw std::runtime_error("number of lights exceeded the max lights");
  uniform("num_lights", (std::uint32_t)lights.size());
  for (std::uint32_t i = 0; i < lights.size(); i++) {
    std::string n = "lights[" + std::to_string(i) + "].";
    uniform((n + "type").c_str(), lights.at(i)->type);
    uniform((n + "direction").c_str(), glm::normalize(lights.at(i)->direction));
    uniform((n + "position").c_str(), lights.at(i)->position);
    uniform((n + "attn").c_str(), lights.at(i)->attn);
  }
  uniforms.load(*this);
  material->load(*this);
}

inline void Shader::uniform(const char *name, float val) {
  auto u = glGetUniformLocation(program, name);
  glUniform1f(u, val);
}

inline void Shader::uniform(const char *name, std::uint32_t val) {
  auto u = glGetUniformLocation(program, name);
  glUniform1ui(u, val);
}

inline void Shader::uniform(const char *name, const glm::vec2 &val) {
  auto u = glGetUniformLocation(program, name);
  glUniform2fv(u, 1, glm::value_ptr(val));
}

inline void Shader::uniform(const char *name, const glm::vec3 &val) {
  auto u = glGetUniformLocation(program, name);
  glUniform3fv(u, 1, glm::value_ptr(val));
}

inline void Shader::uniform(const char *name, const glm::vec4 &val) {
  auto u = glGetUniformLocation(program, name);
  glUniform4fv(u, 1, glm::value_ptr(val));
}

inline void Shader::uniform(const char *name, const glm::mat2 &val) {
  auto u = glGetUniformLocation(program, name);
  glUniformMatrix2fv(u, 1, GL_FALSE, glm::value_ptr(val));
}

inline void Shader::uniform(const char *name, const glm::mat3 &val) {
  auto u = glGetUniformLocation(program, name);
  glUniformMatrix3fv(u, 1, GL_FALSE, glm::value_ptr(val));
}

inline void Shader::uniform(const char *name, const glm::mat4 &val) {
  auto u = glGetUniformLocation(program, name);
  glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(val));
}

inline MVPShaderUniforms::MVPShaderUniforms(const glm::mat4 &model,
                                            const glm::mat4 &view,
                                            const glm::mat4 &projection)
    : model(model), view(view), projection(projection) {}

inline void MVPShaderUniforms::load(Shader &shader) const {
  shader.uniform("model", model);
  shader.uniform("view", view);
  shader.uniform("projection", projection);
}

inline void Shader::on_use() {}

inline Material::~Material() {}

GLE_NAMESPACE_END