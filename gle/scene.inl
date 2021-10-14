GLE_NAMESPACE_BEGIN

inline Scene::Scene() : _objects(), _lights() {}

inline void Scene::init() {
  for (auto &texture : _textures) {
    texture->init();
  }

  for (auto &mesh : _meshs) {
    mesh->init_buffers();
  }

  for (auto &shader : _shaders) {
    shader->load();
  }
}

template <class... Args> inline Camera &Scene::make_camera(Args &&...args) {
  _camera = std::make_unique<Camera>(std::forward<Args>(args)...);
  return *_camera;
}

template <class... Args> inline Object &Scene::make_object(Args &&...args) {
  _objects.push_back(std::make_unique<Object>(std::forward<Args>(args)...));
  return *_objects.back();
}

template <class... Args> inline Light &Scene::make_light(Args &&...args) {
  _lights.push_back(std::make_unique<Light>(std::forward<Args>(args)...));
  return *_lights.back();
}

template <class T, class... Args>
inline Texture &Scene::make_texture(Args &&...args) {
  _textures.push_back(std::make_unique<T>(std::forward<Args>(args)...));
  return *_textures.back();
}

template <class T, class... Args>
inline Material &Scene::make_material(Args &&...args) {
  _materials.push_back(std::make_unique<T>(std::forward<Args>(args)...));
  return *_materials.back();
}

template <class T, class... Args>
inline Shader &Scene::make_shader(Args &&...args) {
  _shaders.push_back(std::make_unique<T>(std::forward<Args>(args)...));
  return *_shaders.back();
}

inline Mesh &Scene::mesh(std::unique_ptr<Mesh> mesh) {
  _meshs.push_back(std::move(mesh));
  return *_meshs.back();
}

inline const Camera &Scene::camera() const { return *_camera; }
inline Camera &Scene::camera() { return *_camera; }

inline const std::vector<std::unique_ptr<Object>> &Scene::objects() const {
  return _objects;
}

inline const std::vector<std::unique_ptr<Light>> &Scene::lights() const {
  return _lights;
}

inline const std::optional<GLuint> &Scene::shadow_map() const {
  return _shadow_map;
}

inline void Scene::shadow_map(GLuint tex) { _shadow_map = tex; }

inline const std::optional<glm::mat4> &Scene::light_space_matrix() const {
  return _light_space_matrix;
}

inline void Scene::light_space_matrix(const glm::mat4 &mat) {
  _light_space_matrix = mat;
}

GLE_NAMESPACE_END