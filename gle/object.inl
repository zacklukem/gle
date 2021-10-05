GLE_NAMESPACE_BEGIN
inline Object::Object(std::shared_ptr<Shader> shader,
                      std::shared_ptr<Mesh> mesh)
    : _shader(shader), _mesh(mesh), _position(0), _rotation(glm::vec3(0)),
      _scale(1) {
  update_model_matrix();
}

inline Object::Object(std::shared_ptr<Shader> shader,
                      std::shared_ptr<Mesh> mesh, const glm::vec3 &position,
                      const glm::quat &rotation, const glm::vec3 &scale)
    : _shader(shader), _mesh(mesh), _position(position), _rotation(rotation),
      _scale(scale) {
  update_model_matrix();
}
inline std::shared_ptr<Shader> Object::shader() { return _shader; }
inline std::shared_ptr<Mesh> Object::mesh() { return _mesh; }

inline const glm::vec3 &Object::position() const { return _position; }
inline const glm::vec3 &Object::scale() const { return _scale; }
inline const glm::quat &Object::rotation() const { return _rotation; }
inline glm::vec3 Object::rotation_euler() const {
  return glm::eulerAngles(_rotation);
}

inline void Object::position(const glm::vec3 &value) {
  _position = value;
  update_model_matrix();
}

inline void Object::scale(const glm::vec3 &value) {
  _scale = value;
  update_model_matrix();
}

inline void Object::rotation(const glm::vec3 &value) {
  _rotation = value;
  update_model_matrix();
}

inline void Object::rotation(const glm::quat &value) {
  _rotation = value;
  update_model_matrix();
}

inline const glm::mat4 &Object::model_matrix() const { return _model; }

inline void Object::update_model_matrix() {
  auto translate = glm::translate(glm::mat4(1), _position);
  auto scale = glm::scale(glm::mat4(1), _scale);
  auto rotate = glm::toMat4(_rotation);
  _model = rotate * translate * scale;
}

GLE_NAMESPACE_END