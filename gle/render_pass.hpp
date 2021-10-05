#ifndef GLE_RENDER_PASS_HPP
#define GLE_RENDER_PASS_HPP

#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

/// @brief A render pass. Called each frame by the Window class
///
class RenderPass {
public:
  /// @brief Render this pass
  ///
  inline void do_render(std::shared_ptr<Camera> camera);

  /// @brief Initialize this pass (called by Window::load() )
  ///
  inline virtual void load();

protected:
  /// @brief Render this pass (implementation)
  ///
  /// @pure
  /// @param camera
  virtual void render(std::shared_ptr<Camera> camera) = 0;
};

GLE_NAMESPACE_END

#include "render_pass.inl"

#endif // GLE_RENDER_PASS_HPP
