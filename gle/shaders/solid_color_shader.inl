GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *solid_color_vertex_shader = R"(
#version 410

layout (location = 0) in vec3 position;

void main() {
  gl_Position = vec4(position, 1.0);
}
)";
const char *solid_color_fragment_shader = R"(
#version 410

out vec4 FragColor;

void main() {
  FragColor = vec4(1.0, 0.3, 0.3, 1.0);
}
)";
} // namespace __internal__

inline SolidColorShader::SolidColorShader()
    : Shader(__internal__::solid_color_vertex_shader,
             __internal__::solid_color_fragment_shader) {}

GLE_NAMESPACE_END