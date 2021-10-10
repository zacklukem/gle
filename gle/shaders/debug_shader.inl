GLE_NAMESPACE_BEGIN

namespace __internal__ {
const char *debug_vertex_shader = R"(
out VS_OUT {
  vec3 normal;
  vec3 tangent;
  vec3 bitangent;
} vs_out;

void main() {
  gl_Position = view * model * vec4(position, 1.0);
  mat3 normal_matrix = mat3(transpose(inverse(view * model)));
  vs_out.normal = normalize(vec3(vec4(normal_matrix * normal, 0.0)));
  vs_out.tangent = normalize(vec3(vec4(normal_matrix * tangent, 0.0)));
  vs_out.bitangent = normalize(vec3(vec4(normal_matrix * bitangent, 0.0)));
}
)";

const char *debug_fragment_shader = R"(
in vec3 vector_color;

void main() {
  FragColor = vec4(vector_color, 1.0);
}
)";

const char *debug_geometry_shader = R"(
#version 410
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
  vec3 normal;
  vec3 tangent;
  vec3 bitangent;
} gs_in[];

const float MAGNITUDE = 0.2;

#define NORMAL_COLOR vec3(1.0, 0.0, 0.0)
#define TANGENT_COLOR vec3(0.0, 1.0, 0.0)
#define BITANGENT_COLOR vec3(0.0, 0.0, 1.0)

out vec3 vector_color;

uniform mat4 projection;

void generate_line(int index) {
  gl_Position = projection * gl_in[index].gl_Position;
  vector_color = NORMAL_COLOR;
  EmitVertex();
  gl_Position = projection * (gl_in[index].gl_Position +
                              vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
  vector_color = NORMAL_COLOR;
  EmitVertex();
  EndPrimitive();
  gl_Position = projection * gl_in[index].gl_Position;
  vector_color = TANGENT_COLOR;
  EmitVertex();
  gl_Position = projection * (gl_in[index].gl_Position +
                              vec4(gs_in[index].tangent, 0.0) * MAGNITUDE);
  vector_color = TANGENT_COLOR;
  EmitVertex();
  EndPrimitive();
  gl_Position = projection * gl_in[index].gl_Position;
  vector_color = BITANGENT_COLOR;
  EmitVertex();
  gl_Position = projection * (gl_in[index].gl_Position +
                              vec4(gs_in[index].bitangent, 0.0) * MAGNITUDE);
  vector_color = BITANGENT_COLOR;
  EmitVertex();
  EndPrimitive();
}

void main() {
  generate_line(0); // first vertex normal
  generate_line(1); // second vertex normal
  generate_line(2); // third vertex normal
}
)";
} // namespace __internal__

inline void DebugMaterial::load(const Shader &) const {};

inline DebugShader::DebugShader()
    : Shader(__internal__::debug_vertex_shader,
             __internal__::debug_fragment_shader,
             std::string(__internal__::debug_geometry_shader)) {}

GLE_NAMESPACE_END