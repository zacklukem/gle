GLE_NAMESPACE_BEGIN

inline void RenderPass::do_render(const Scene &scene) const { render(scene); }

inline void RenderPass::load(Scene &) {}

inline RenderPass::~RenderPass() {}

GLE_NAMESPACE_END