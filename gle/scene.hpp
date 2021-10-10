#ifndef GLE_SCENE_HPP
#define GLE_SCENE_HPP

#include <gle/camera.hpp>
#include <gle/common.hpp>
#include <optional>

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

  inline std::optional<GLuint> shadow_map() const;

  inline void shadow_map(GLuint tex);

  inline std::optional<glm::mat4> light_space_matrix() const;

  inline void light_space_matrix(const glm::mat4 &mat);

private:
  std::shared_ptr<Camera> _camera;
  std::vector<std::shared_ptr<Object>> _objects;
  std::vector<std::shared_ptr<Light>> _lights;
  std::optional<GLuint> _shadow_map;
  std::optional<glm::mat4> _light_space_matrix;
};

GLE_NAMESPACE_END

#endif // GLE_SCENE_HPP
