GLE_NAMESPACE_BEGIN

inline void ObjectRenderPass::load() {
  for (auto object : objects) {
    // object->shader()->load();
    object->mesh()->init_buffers();
  }
}

inline void ObjectRenderPass::render() {
  for (auto object : objects) {
    object->shader()->use();

    object->mesh()->bind_buffers();

    glDrawElements(GL_TRIANGLES, object->mesh()->num_elements(),
                   GL_UNSIGNED_INT, (void *)0);

    object->mesh()->post_draw();
  }
}

inline void ObjectRenderPass::add_object(std::shared_ptr<Object> object) {
  objects.push_back(object);
}

GLE_NAMESPACE_END