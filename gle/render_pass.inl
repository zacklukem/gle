GLE_NAMESPACE_BEGIN

inline void RenderPass::do_render(std::shared_ptr<Camera> camera) {
  render(camera);
}

inline void RenderPass::load() {}

inline RenderPass::~RenderPass() {}

GLE_NAMESPACE_END