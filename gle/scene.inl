GLE_NAMESPACE_BEGIN

inline Scene::Scene() : _objects(), _lights() {}

inline void Scene::init() {
  for (auto object : _objects) {
    if (!object->shader()->is_loaded()) object->shader()->load();
    object->mesh()->init_buffers();
  }
}

inline void Scene::camera(std::shared_ptr<Camera> camera) { _camera = camera; }

inline void Scene::add_object(std::shared_ptr<Object> object) {
  _objects.push_back(object);
}

inline void Scene::add_light(std::shared_ptr<Light> light) {
  _lights.push_back(light);
}

inline std::shared_ptr<Camera> Scene::camera() const { return _camera; }

inline const std::vector<std::shared_ptr<Object>> &Scene::objects() const {
  return _objects;
}

inline const std::vector<std::shared_ptr<Light>> &Scene::lights() const {
  return _lights;
}

GLE_NAMESPACE_END