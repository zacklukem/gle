GLE_NAMESPACE_BEGIN

inline Object::Object(std::shared_ptr<Shader> shader,
                      std::shared_ptr<Mesh> mesh)
    : _shader(shader), _mesh(mesh) {}

inline std::shared_ptr<Shader> Object::shader() { return _shader; }
inline std::shared_ptr<Mesh> Object::mesh() { return _mesh; }

GLE_NAMESPACE_END