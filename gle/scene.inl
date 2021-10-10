GLE_NAMESPACE_BEGIN

inline Scene::Scene() : _objects(), _lights() {}

inline void Scene::init() {
  for (auto &object : _objects) {
    if (!object->shader().is_loaded()) object->shader().load();
    object->mesh().init_buffers();
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