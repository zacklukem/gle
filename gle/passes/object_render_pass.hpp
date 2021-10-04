#ifndef GLE_PASSES_OBJECT_RENDER_PASS_HPP
#define GLE_PASSES_OBJECT_RENDER_PASS_HPP

#include <gle/common.hpp>
#include <gle/object.hpp>
#include <gle/render_pass.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

class ObjectRenderPass : public RenderPass {
public:
  inline void add_object(std::shared_ptr<Object>);

  virtual void render() override;
  virtual void load() override;

private:
  std::vector<std::shared_ptr<Object>> objects;
};

GLE_NAMESPACE_END

#include "object_render_pass.inl"

#endif // GLE_PASSES_OBJECT_RENDER_PASS_HPP
