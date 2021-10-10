#ifndef GLE_RENDER_PASS_HPP
#define GLE_RENDER_PASS_HPP

#include <gle/common.hpp>
#include <gle/gl.hpp>
#include <gle/scene.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

/// @brief A render pass. Called each frame by the Window class
///
class RenderPass {
public:
  inline virtual ~RenderPass();

  /// @brief Render this pass
  ///
  inline void do_render(std::shared_ptr<const Scene> scene) const;

  /// @brief Initialize this pass (called by Window::load() )
  ///
  inline virtual void load(std::shared_ptr<Scene> scene);

protected:
  /// @brief Render this pass (implementation)
  ///
  /// @pure
  /// @param scene
  virtual void render(std::shared_ptr<const Scene> scene) const = 0;
};

GLE_NAMESPACE_END

#endif // GLE_RENDER_PASS_HPP
