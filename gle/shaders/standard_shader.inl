GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *standard_vertex_shader = R"(
out vec3 frag_normal;
out vec3 frag_position;
out vec2 frag_uv;

void main() {
  frag_uv = uv;
  frag_normal = mat3(transpose(inverse(model))) * normal;
  gl_Position = projection * view * model * vec4(position, 1.0);
  frag_position = (model * vec4(position, 1.0)).xyz;
}
)";

const char *standard_fragment_shader = R"(
in vec3 frag_normal;
in vec3 frag_position;
in vec2 frag_uv;

struct Material {
  sampler2D color;
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
    attn += dir_light(lights[i], view_dir, normal);
  }
  FragColor = vec4(texture(mat.color, frag_uv).xyz * attn, 1.0);
}
)";
} // namespace __internal__

inline StandardMaterial::StandardMaterial(std::shared_ptr<Texture> color,
                                          float diffuse, float specular)
    : color(color), diffuse(diffuse), specular(specular) {}

inline void StandardMaterial::load(Shader &shader) const {
  shader.uniform("mat.color", 0, color);
  shader.uniform("mat.diffuse", diffuse);
  shader.uniform("mat.specular", specular);
}

inline StandardShader::StandardShader()
    : Shader(__internal__::standard_vertex_shader,
             __internal__::standard_fragment_shader) {}

GLE_NAMESPACE_END