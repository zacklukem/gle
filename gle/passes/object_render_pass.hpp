#ifndef GLE_PASSES_OBJECT_RENDER_PASS_HPP
#define GLE_PASSES_OBJECT_RENDER_PASS_HPP

#include <gle/camera.hpp>
#include <gle/common.hpp>
#include <gle/object.hpp>
#include <gle/render_pass.hpp>
#include <gle/scene.hpp>
#include <gle/shaders/debug_shader.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

class ObjectRenderPass : public RenderPass {
public:
  inline virtual void render(std::shared_ptr<const Scene> scene) const override;
  inline virtual void load(std::shared_ptr<Scene> scene) override;

private:
#ifdef GLE_DEBUG_LINES
  std::shared_ptr<DebugShader> debug_shader;
#endif
};

GLE_NAMESPACE_END

#endif // GLE_PASSES_OBJECT_RENDER_PASS_HPP
