GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *solid_color_vertex_shader = R"(
out vec3 frag_normal;
out vec3 frag_position;
out vec4 frag_position_light_space;

void main() {
  frag_normal = mat3(transpose(inverse(model))) * normal;
  gl_Position = projection * view * model * vec4(position, 1.0);
  frag_position = (model * vec4(position, 1.0)).xyz;
  frag_position_light_space = light_space_matrix * vec4(frag_position, 1.0);
}
)";
const char *solid_color_fragment_shader = R"(
in vec3 frag_normal;
in vec3 frag_position;
in vec4 frag_position_light_space;

struct Material {
  vec3 color;
  float diffuse;
  float specular;
};

uniform Material mat;

vec3 dir_light(in Light light, in vec3 view_dir, in vec3 normal) {
  vec3 diffuse = max(dot(normal, -normalize(light.direction)), 0)
                 * light.attn * mat.diffuse;
  vec3 reflect_dir = reflect(light.direction, normal);
  vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0), 32) * light.attn
                  * mat.specular;
  return diffuse + specular;
}


float shadow(in vec3 normal, in vec3 light_dir) {
  vec3 proj_coords = frag_position_light_space.xyz / frag_position_light_space.w;
  proj_coords = proj_coords * 0.5 + 0.5;
  if(proj_coords.z > 1.0)
    return 0.0;
  // float closest_depth = texture(shadow_map, proj_coords.xy).r;
  float current_depth = proj_coords.z;

  float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);

  float shadow = 0.0;
  vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
  for(int x = -1; x <= 1; x++) {
    for(int y = -1; y <= 1; y++) {
      float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
      shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
    }
  }
  shadow /= 9.0;

  return shadow;
}

void main() {
  vec3 attn = vec3(0);
  vec3 view_dir = normalize(camera.origin - frag_position);
  vec3 normal = normalize(frag_normal);
  vec3 light_dir = vec3(0);
  for (uint i = 0; i < num_lights; i++) {
    float point_attn = 1.0;
    if (lights[i].type == POINT_LIGHT) {
      float dist = distance(frag_position, lights[i].position);
      point_attn = 1.0 / (1.0 + dist * dist);
    } else if (lights[i].type == DIRECTIONAL_LIGHT && light_dir == vec3(0)) {
      light_dir = normalize(lights[i].direction);
    }
    attn += dir_light(lights[i], view_dir, normal) * point_attn;
  }
  attn *= 1.0 - shadow(normal, light_dir);
  attn += vec3(0.2);
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