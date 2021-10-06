GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *solid_color_vertex_shader = R"(
#version 410

in vec3 position;
in vec3 normal;

out float attn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  attn = max(dot(normalize(normal), normalize(vec3(-1.0, -1.0, 0.0))), 0.3);
  gl_Position = projection * view * model * vec4(position, 1.0);
}
)";
const char *solid_color_fragment_shader = R"(
#version 410

out vec4 FragColor;
in float attn;

uniform vec3 color;

void main() {
  FragColor = vec4(color * attn, 1.0);
}
)";
} // namespace __internal__

inline SolidColorShader::SolidColorShader(const glm::vec3 &color)
    : Shader(__internal__::solid_color_vertex_shader,
             __internal__::solid_color_fragment_shader),
      color(color) {}

inline void SolidColorShader::on_use() { uniform("color", color); }

GLE_NAMESPACE_END