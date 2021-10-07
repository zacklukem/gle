GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *solid_color_vertex_shader = R"(
out vec3 frag_normal;

void main() {
  frag_normal = normal;
  gl_Position = projection * view * model * vec4(position, 1.0);
}
)";
const char *solid_color_fragment_shader = R"(
in vec3 frag_normal;

uniform vec3 color;

vec3 dir_light(in Light light) {
  return dot(normalize(frag_normal), light.direction)
             * light.attn;
}

void main() {
  vec3 attn = vec3(0.2); // ambient
  for (uint i = 0; i < num_lights; i++) {
    attn += dir_light(lights[i]);
  }
  FragColor = vec4(color * attn, 1.0);
}
)";
} // namespace __internal__
inline SolidColorMaterial::SolidColorMaterial(const glm::vec3 &color)
    : color(color) {}

inline void SolidColorMaterial::load(Shader &shader) const {
  shader.uniform("color", color);
}

inline SolidColorShader::SolidColorShader()
    : Shader(__internal__::solid_color_vertex_shader,
             __internal__::solid_color_fragment_shader) {}

GLE_NAMESPACE_END