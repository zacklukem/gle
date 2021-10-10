#ifndef GLE_SCENE_HPP
#define GLE_SCENE_HPP

#include <gle/camera.hpp>
#include <gle/common.hpp>
#include <optional>

GLE_NAMESPACE_BEGIN

class Scene {
public:
  inline Scene();
  template <class... Args> inline Object &make_object(Args &&...args);
  template <class... Args> inline Light &make_light(Args &&...args);

  inline void init();

  template <class... Args> inline Camera &make_camera(Args &&...args);

  inline const Camera &camera() const;
  inline Camera &camera();

  inline const std::vector<std::unique_ptr<Object>> &objects() const;

  inline const std::vector<std::unique_ptr<Light>> &lights() const;

  inline const std::optional<GLuint> &shadow_map() const;

  inline void shadow_map(GLuint tex);

  inline const std::optional<glm::mat4> &light_space_matrix() const;

  inline void light_space_matrix(const glm::mat4 &mat);

private:
  std::unique_ptr<Camera> _camera;
  std::vector<std::unique_ptr<Object>> _objects;
  std::vector<std::unique_ptr<Light>> _lights;
  std::optional<GLuint> _shadow_map;
  std::optional<glm::mat4> _light_space_matrix;
};

GLE_NAMESPACE_END

#endif // GLE_SCENE_HPP
