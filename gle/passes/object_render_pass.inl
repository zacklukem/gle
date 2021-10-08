#include <glm/gtc/matrix_transform.hpp>

GLE_NAMESPACE_BEGIN

inline void ObjectRenderPass::load(std::shared_ptr<const Scene>) {
#ifdef GLE_DEBUG_LINES
  debug_shader = std::make_shared<DebugShader>();
  debug_shader->load();
#endif
}

inline void ObjectRenderPass::render(std::shared_ptr<const Scene> scene) const {
  for (auto object : scene->objects()) {
    auto uniforms = MVPShaderUniforms(object->model_matrix(),
                                      scene->camera()->view_matrix(),
                                      scene->camera()->projection_matrix());

    object->shader()->use(scene->lights(), uniforms, object->material(),
                          scene->camera());
    object->mesh()->bind_buffers();

    glDrawElements(GL_TRIANGLES, object->mesh()->num_elements(),
                   GL_UNSIGNED_INT, (void *)0);

#ifdef GLE_DEBUG_LINES
    debug_shader->use(scene->lights(), uniforms, object->material(),
                      scene->camera());

    glDrawElements(GL_TRIANGLES, object->mesh()->num_elements(),
                   GL_UNSIGNED_INT, (void *)0);
#endif

    object->mesh()->post_draw();
  }
}

GLE_NAMESPACE_END