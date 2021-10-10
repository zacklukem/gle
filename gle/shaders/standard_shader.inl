GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *standard_vertex_shader = R"(
out vec3 frag_normal;
out vec3 frag_position;
out vec4 frag_position_light_space;
out vec2 frag_uv;

out mat3 tbn;

void main() {
  frag_uv = uv;
  frag_normal = mat3(transpose(inverse(model))) * normal;
  gl_Position = projection * view * model * vec4(position, 1.0);
  frag_position = (model * vec4(position, 1.0)).xyz;

  frag_position_light_space = light_space_matrix * vec4(frag_position, 1.0);

  vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
  vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
  vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
  tbn = mat3(T, B, N);
}
)";

const char *standard_fragment_shader = R"(
in vec3 frag_normal;
in vec3 frag_position;
in vec4 frag_position_light_space;
in vec2 frag_uv;

in mat3 tbn;

uniform sampler2D color_tex;
uniform sampler2D normal_tex;

struct Material {
  float diffuse;
  float specular;
};

uniform Material mat;

vec3 light(in Light light, in vec3 view_dir, in vec3 normal) {
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

  float bias = max(0.01 * (1.0 - dot(normal, light_dir)), 0.005);
  // float bias = 0.005;

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
  vec3 attn = vec3(0.0);
  vec3 view_dir = normalize(camera.origin - frag_position);
  // vec3 normal = normalize(frag_normal);
  vec3 normal = texture(normal_tex, frag_uv).rgb;
  normal = normal * 2.0 - 1.0;
  normal = normalize(tbn * normal);
  vec3 light_dir = vec3(0);
  for (uint i = 0; i < num_lights; i++) {
    float point_attn = 1.0;
    if (lights[i].type == POINT_LIGHT) {
      float dist = distance(frag_position, lights[i].position);
      point_attn = 1.0 / (1.0 + dist * dist);
    } else if (lights[i].type == DIRECTIONAL_LIGHT && light_dir == vec3(0)) {
      light_dir = normalize(lights[i].direction);
    }
    attn += light(lights[i], view_dir, normal) * point_attn;
  }
  attn *= 1.0 - shadow(normalize(frag_normal), light_dir);
  attn += vec3(0.2); // ambient
  FragColor = vec4(texture(color_tex, frag_uv).rgb * attn, 1.0);
}
)";
} // namespace __internal__

inline StandardMaterial::StandardMaterial(std::shared_ptr<Texture> color,
                                          std::shared_ptr<Texture> normal,
                                          float diffuse, float specular)
    : color(color), normal(normal), diffuse(diffuse), specular(specular) {}

inline void StandardMaterial::preload(const Shader &) const {}

inline void StandardMaterial::load(const Shader &shader) const {
  shader.uniform("color_tex", 0, color);
  shader.uniform("normal_tex", 1, normal);
  shader.uniform("mat.diffuse", diffuse);
  shader.uniform("mat.specular", specular);
}

inline StandardShader::StandardShader()
    : Shader(__internal__::standard_vertex_shader,
             __internal__::standard_fragment_shader) {}

GLE_NAMESPACE_END