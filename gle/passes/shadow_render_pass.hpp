#ifndef GLE_PASSES_SHADOW_RENDER_PASS_HPP
#define GLE_PASSES_SHADOW_RENDER_PASS_HPP

#include <gle/common.hpp>
#include <gle/render_pass.hpp>
#include <gle/shader.hpp>

GLE_NAMESPACE_BEGIN

class ShadowRenderPass : public RenderPass {
public:
  inline ShadowRenderPass();
  inline virtual void load(Scene &scene) override;
  inline virtual void render(const Scene &scene) const override;

private:
  std::unique_ptr<Shader> shader;
  GLuint depth_fbo;
  GLuint depth_tex;
};

GLE_NAMESPACE_END

#endif // GLE_PASSES_SHADOW_RENDER_PASS_HPP
