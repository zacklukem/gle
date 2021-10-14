#ifndef GLE_SCENE_HPP
#define GLE_SCENE_HPP

#include <gle/camera.hpp>
#include <gle/common.hpp>
#include <gle/shader.hpp>
#include <gle/texture.hpp>
#include <optional>

GLE_NAMESPACE_BEGIN

class Scene {
public:
  Scene(Scene &) = delete;
  Scene(Scene &&) = delete;
  Scene(const Scene &) = delete;
  Scene(const Scene &&) = delete;

  inline Scene();
  template <class... Args> inline Object &make_object(Args &&...args);
  template <class... Args> inline Light &make_light(Args &&...args);
  template <class... Args> inline Camera &make_camera(Args &&...args);
  template <class T, class... Args>
  inline Texture &make_texture(Args &&...args);
  template <class T, class... Args>
  inline Material &make_material(Args &&...args);
  template <class T, class... Args> inline Shader &make_shader(Args &&...args);

  inline Mesh &mesh(std::unique_ptr<Mesh> mesh);

  inline void init();

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
  std::vector<std::unique_ptr<Shader>> _shaders;
  std::vector<std::unique_ptr<Material>> _materials;
  std::vector<std::unique_ptr<Texture>> _textures;
  std::vector<std::unique_ptr<Mesh>> _meshs;
  std::optional<GLuint> _shadow_map;
  std::optional<glm::mat4> _light_space_matrix;
};

GLE_NAMESPACE_END

#endif // GLE_SCENE_HPP
