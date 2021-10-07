#include <glm/gtc/matrix_transform.hpp>

GLE_NAMESPACE_BEGIN

inline void ObjectRenderPass::load() {
  for (auto object : objects) {
    object->mesh()->init_buffers();
  }
}

inline void ObjectRenderPass::render(std::shared_ptr<Camera> camera) {
  for (auto object : objects) {
    auto uniforms =
        MVPShaderUniforms(object->model_matrix(), camera->view_matrix(),
                          camera->projection_matrix());

    object->shader()->use(lights, uniforms, object->material());
    object->mesh()->bind_buffers();

    glDrawElements(GL_TRIANGLES, object->mesh()->num_elements(),
                   GL_UNSIGNED_INT, (void *)0);

    object->mesh()->post_draw();
  }
}

inline void ObjectRenderPass::add_object(std::shared_ptr<Object> object) {
  objects.push_back(object);
}

inline void ObjectRenderPass::add_light(std::shared_ptr<Light> light) {
  lights.push_back(light);
}

GLE_NAMESPACE_END