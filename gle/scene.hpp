#ifndef GLE_SCENE_HPP
#define GLE_SCENE_HPP

#include <gle/camera.hpp>
#include <gle/common.hpp>

GLE_NAMESPACE_BEGIN

class Scene {
public:
  inline Scene();
  inline void add_object(std::shared_ptr<Object>);
  inline void add_light(std::shared_ptr<Light>);

  inline void init();

  /// @brief Set the camera object for this window
  ///
  /// @param camera
  inline void camera(std::shared_ptr<Camera> camera);

  inline std::shared_ptr<Camera> camera() const;

  inline const std::vector<std::shared_ptr<Object>> &objects() const;

  inline const std::vector<std::shared_ptr<Light>> &lights() const;

private:
  std::shared_ptr<Camera> _camera;
  std::vector<std::shared_ptr<Object>> _objects;
  std::vector<std::shared_ptr<Light>> _lights;
};

GLE_NAMESPACE_END

#include "scene.inl"

#endif // GLE_SCENE_HPP
