GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *solid_color_vertex_shader = R"(
#version 410

in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(position * 0.5, 1.0);
}
)";
const char *solid_color_fragment_shader = R"(
#version 410

out vec4 FragColor;

uniform vec3 color;

void main() {
  FragColor = vec4(color, 1.0);
}
)";
} // namespace __internal__

inline SolidColorShader::SolidColorShader(const glm::vec3 &color)
    : Shader(__internal__::solid_color_vertex_shader,
             __internal__::solid_color_fragment_shader),
      color(color) {}

inline void SolidColorShader::on_use() { uniform("color", color); }

GLE_NAMESPACE_END