#include <glm/gtc/matrix_transform.hpp>

GLE_NAMESPACE_BEGIN

inline void ObjectRenderPass::load(Scene &) {
#ifdef GLE_DEBUG_LINES
  debug_shader = std::make_unique<DebugShader>();
  debug_shader->load();
#endif
}

inline void ObjectRenderPass::render(const Scene &scene) const {
  for (const auto &object : scene.objects()) {
    auto uniforms =
        MVPShaderUniforms(object->model_matrix(), scene.camera().view_matrix(),
                          scene.camera().projection_matrix());

    object->shader().use(scene, uniforms, object->material());
    object->mesh().draw();

#ifdef GLE_DEBUG_LINES
    debug_shader->use(scene, uniforms, object->material());
    object->mesh().draw();
#endif
  }
}

GLE_NAMESPACE_END