#ifndef GLE_PASSES_OBJECT_RENDER_PASS_HPP
#define GLE_PASSES_OBJECT_RENDER_PASS_HPP

#include <gle/camera.hpp>
#include <gle/common.hpp>
#include <gle/object.hpp>
#include <gle/render_pass.hpp>
#include <gle/shaders/debug_shader.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

class ObjectRenderPass : public RenderPass {
public:
  inline void add_object(std::shared_ptr<Object>);
  inline void add_light(std::shared_ptr<Light>);

  virtual void render(std::shared_ptr<const Camera> camera) override;
  virtual void load() override;

private:
  std::vector<std::shared_ptr<Object>> objects;
  std::vector<std::shared_ptr<Light>> lights;
#ifdef GLE_DEBUG_LINES
  std::shared_ptr<DebugShader> debug_shader;
#endif
};

GLE_NAMESPACE_END

#include "object_render_pass.inl"

#endif // GLE_PASSES_OBJECT_RENDER_PASS_HPP
