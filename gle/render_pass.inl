GLE_NAMESPACE_BEGIN

inline void RenderPass::do_render(std::shared_ptr<const Scene> scene) const {
  render(scene);
}

inline void RenderPass::load(std::shared_ptr<const Scene>) {}

inline RenderPass::~RenderPass() {}

GLE_NAMESPACE_END