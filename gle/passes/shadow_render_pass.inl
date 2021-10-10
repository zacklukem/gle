#include <glm/gtc/type_ptr.hpp>

GLE_NAMESPACE_BEGIN

namespace __internal__ {
const GLuint shadow_width = 1024;
const GLuint shadow_height = 1024;

const char *shadow_render_pass_vertex = R"(
#version 410

in vec3 position;

uniform mat4 light_space_matrix;
uniform mat4 model;

void main() {
  gl_Position = light_space_matrix * model * vec4(position, 1.0);
}
)";
const char *shadow_render_pass_fragment = R"(
#version 410

void main() {}
)";
} // namespace __internal__

inline ShadowRenderPass::ShadowRenderPass() {
  shader = std::make_shared<Shader>(__internal__::shadow_render_pass_vertex,
                                    __internal__::shadow_render_pass_fragment,
                                    false);
}

inline void ShadowRenderPass::load(std::shared_ptr<Scene> scene) {
  shader->load();

  // TODO: abstract away opengl calls here

  glGenFramebuffers(1, &depth_fbo);
  // create depth texture
  glGenTextures(1, &depth_tex);
  glBindTexture(GL_TEXTURE_2D, depth_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, __internal__::shadow_width,
               __internal__::shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  auto border = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                   glm::value_ptr(border));
  // attach depth texture as FBO's depth buffer
  glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depth_tex, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  scene->shadow_map(depth_tex);

  std::shared_ptr<Light> light = nullptr;

  for (auto c_light : scene->lights()) {
    if (c_light->type == LightType::DIRECTIONAL_LIGHT) light = c_light;
  }

  if (!light)
    throw std::runtime_error(
        "must have at least one directional light for shadows");

  float near_plane = 1.0f, far_plane = 10.5f;
  // TODO: get scene objects and calculate bounding box to solve for the ideal
  // light position
  auto light_position = glm::vec3(-1, 8, -1);
  auto light_projection =
      glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  auto light_view =
      glm::lookAt(light_position, light_position + light->direction,
                  glm::vec3(0.0, 1.0, 0.0));
  auto light_space_matrix = light_projection * light_view;

  scene->light_space_matrix(light_space_matrix);
}

inline void ShadowRenderPass::render(std::shared_ptr<const Scene> scene) const {
  glViewport(0, 0, __internal__::shadow_width, __internal__::shadow_height);
  glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
  glClear(GL_DEPTH_BUFFER_BIT);

  for (auto object : scene->objects()) {

    shader->use();
    shader->uniform("light_space_matrix", scene->light_space_matrix().value());
    shader->uniform("model", object->model_matrix());

    object->mesh()->draw();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLE_NAMESPACE_END