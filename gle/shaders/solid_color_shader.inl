GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *solid_color_vertex_shader = R"(
out vec3 frag_normal;
out vec3 frag_position;

void main() {
  frag_normal = mat3(transpose(inverse(model))) * normal;
  gl_Position = projection * view * model * vec4(position, 1.0);
  frag_position = (model * vec4(position, 1.0)).xyz;
}
)";
const char *solid_color_fragment_shader = R"(
in vec3 frag_normal;
in vec3 frag_position;

struct Material {
  vec3 color;
  float diffuse;
  float specular;
};

uniform Material mat;

vec3 dir_light(in Light light, in vec3 view_dir, in vec3 normal) {
  vec3 diffuse = max(dot(normal, normalize(light.direction)), 0)
                 * light.attn * mat.diffuse;
  vec3 reflect_dir = reflect(-light.direction, normal);
  vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0), 32) * light.attn
                  * mat.specular;
  return diffuse + specular;
}

void main() {
  vec3 attn = vec3(0.1); // ambient
  vec3 view_dir = normalize(camera.origin - frag_position);
  vec3 normal = normalize(frag_normal);
  for (uint i = 0; i < num_lights; i++) {
    float point_attn = 1.0;
    if (lights[i].type == POINT_LIGHT) {
      float dist = distance(frag_position, lights[i].position);
      point_attn = 1.0 / (1.0 + dist * dist);
    }
    attn += dir_light(lights[i], view_dir, normal) * point_attn;
  }
  FragColor = vec4(mat.color * attn, 1.0);
}
)";
} // namespace __internal__
inline SolidColorMaterial::SolidColorMaterial(const glm::vec3 &color,
                                              float diffuse, float specular)
    : color(color), diffuse(diffuse), specular(specular) {}

inline void SolidColorMaterial::load(const Shader &shader) const {
  shader.uniform("mat.color", color);
  shader.uniform("mat.diffuse", diffuse);
  shader.uniform("mat.specular", specular);
}

inline SolidColorShader::SolidColorShader()
    : Shader(__internal__::solid_color_vertex_shader,
             __internal__::solid_color_fragment_shader) {}

GLE_NAMESPACE_END